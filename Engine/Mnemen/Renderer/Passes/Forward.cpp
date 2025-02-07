//
// > Notice: Amélie Heinrich @ 2025
// > Create Time: 2025-02-03 22:01:27
//

#include "Forward.hpp"

#include <Asset/AssetManager.hpp>
#include <Core/Application.hpp>
#include <Core/Profiler.hpp>

Forward::Forward(RHI::Ref rhi)
    : RenderPass(rhi)
{
    int width, height;
    Application::Get()->GetWindow()->PollSize(width, height);

    Asset::Handle meshShader = AssetManager::Get("Assets/Shaders/Forward/ForwardMesh.hlsl", AssetType::Shader);
    Asset::Handle fragmentShader = AssetManager::Get("Assets/Shaders/Forward/ForwardFragment.hlsl", AssetType::Shader);

    // Color buffer
    {
        TextureDesc desc = {};
        desc.Width = width;
        desc.Height = height;
        desc.Levels = 1;
        desc.Depth = 1;
        desc.Name = "HDR Color Buffer";
        desc.Usage = TextureUsage::RenderTarget | TextureUsage::Storage | TextureUsage::ShaderResource;
        desc.Format = TextureFormat::RGBA16Float;
       
        auto renderTarget = RendererTools::CreateSharedTexture("HDRColorBuffer", desc);
        renderTarget->AddView(ViewType::RenderTarget);
        renderTarget->AddView(ViewType::ShaderResource);
        renderTarget->AddView(ViewType::Storage);
    }

    // Depth buffer
    {
        TextureDesc desc = {};
        desc.Width = width;
        desc.Height = height;
        desc.Levels = 1;
        desc.Depth = 1;
        desc.Name = "Main Depth Buffer";
        desc.Usage = TextureUsage::DepthTarget;
        desc.Format = TextureFormat::Depth32;
       
        auto depthBuffer = RendererTools::CreateSharedTexture("MainDepthBuffer", desc);
        depthBuffer->AddView(ViewType::DepthTarget);
    }

    // Pipeline
    {
        GraphicsPipelineSpecs specs = {};
        specs.Bytecodes[ShaderType::Mesh] = meshShader->Shader;
        specs.Bytecodes[ShaderType::Fragment] = fragmentShader->Shader;
        specs.Formats.push_back(TextureFormat::RGBA8);
        specs.Cull = CullMode::None;
        specs.Fill = FillMode::Solid;
        specs.Depth = DepthOperation::Less;
        specs.DepthEnabled = true;
        specs.DepthFormat = TextureFormat::Depth32;
        specs.CCW = false;
        specs.Signature = mRHI->CreateRootSignature({ RootType::PushConstant }, sizeof(int) * 8);

        mPipeline = mRHI->CreateMeshPipeline(specs);
    }

    RendererTools::CreateSharedRingBuffer("CameraRingBuffer", 512);
    RendererTools::CreateSharedSampler("MaterialSampler", SamplerFilter::Linear, SamplerAddress::Wrap, true);
}

void Forward::Render(const Frame& frame, ::Ref<Scene> scene)
{
    PROFILE_FUNCTION();

    auto cameraBuffer = RendererTools::Get("CameraRingBuffer");
    auto colorBuffer = RendererTools::Get("HDRColorBuffer");
    auto depthBuffer = RendererTools::Get("MainDepthBuffer");
    auto sampler = RendererTools::Get("MaterialSampler");

    SceneCamera camera = scene->GetMainCamera();
    cameraBuffer->RBuffer[frame.FrameIndex]->CopyMapped(&camera, sizeof(camera));

    frame.CommandBuffer->BeginMarker("Forward");
    frame.CommandBuffer->Barrier(colorBuffer->Texture, ResourceLayout::ColorWrite);
    frame.CommandBuffer->Barrier(depthBuffer->Texture, ResourceLayout::DepthWrite);
    frame.CommandBuffer->SetViewport(0, 0, frame.Width, frame.Height);
    frame.CommandBuffer->SetRenderTargets({ colorBuffer->GetView(ViewType::RenderTarget) }, depthBuffer->GetView(ViewType::DepthTarget));
    frame.CommandBuffer->ClearRenderTarget(colorBuffer->GetView(ViewType::RenderTarget), 0.0f, 0.0f, 0.0f);
    frame.CommandBuffer->ClearDepth(depthBuffer->GetView(ViewType::DepthTarget));
    frame.CommandBuffer->SetMeshPipeline(mPipeline);
    
    // Draw function for each model
    std::function<void(Frame frame, MeshNode*, Mesh* model, glm::mat4 transform)> drawNode = [&](Frame frame, MeshNode* node, Mesh* model, glm::mat4 transform) {
        if (!node) {
            return;
        }
        glm::mat4 globalTransform = transform * node->Transform;
        for (MeshPrimitive primitive : node->Primitives) {
            MeshMaterial material = model->Materials[primitive.MaterialIndex];

            struct PushConstants {
                int Matrices;
                int VertexBuffer;
                int IndexBuffer;
                int MeshletBuffer;
                int MeshletVertices;
                int MeshletTriangleBuffer;
                int Albedo;
                int Sampler;
            } data = {
                cameraBuffer->Descriptor(ViewType::None, frame.FrameIndex),
                primitive.VertexBuffer->SRV(),
                primitive.IndexBuffer->SRV(),
                primitive.MeshletBuffer->SRV(),
                primitive.MeshletVertices->SRV(),
                primitive.MeshletTriangles->SRV(),
                material.AlbedoView->GetDescriptor().Index,
                sampler->Descriptor()
            };
            frame.CommandBuffer->GraphicsPushConstants(&data, sizeof(data), 0);
            frame.CommandBuffer->DispatchMesh(primitive.MeshletCount);
        }
        if (!node->Children.empty()) {
            for (MeshNode* child : node->Children) {
                drawNode(frame, child, model, globalTransform);
            }
        }
    };

    // Iterate over every mesh. This is messy as hell but fuck it
    auto registry = scene->GetRegistry();
    auto view = registry->view<TransformComponent, MeshComponent>();
    for (auto [entity, transform, mesh]: view.each()) {
        drawNode(frame, mesh.MeshAsset->Mesh.Root, &mesh.MeshAsset->Mesh, transform.Matrix);
    }

    frame.CommandBuffer->Barrier(colorBuffer->Texture, ResourceLayout::Shader);
    frame.CommandBuffer->Barrier(depthBuffer->Texture, ResourceLayout::Common);
    frame.CommandBuffer->EndMarker();
}

void Forward::UI(const Frame& frame)
{

}
