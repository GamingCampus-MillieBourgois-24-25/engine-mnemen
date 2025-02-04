//
// > Notice: Amélie Heinrich @ 2025
// > Create Time: 2025-02-03 22:01:27
//

#include "Forward.hpp"

#include <Asset/AssetManager.hpp>

Forward::Forward(RHI::Ref rhi)
    : RenderPass(rhi)
{
    Asset::Handle meshShader = AssetManager::Get("Assets/Shaders/Forward/ForwardMesh.hlsl", AssetType::Shader);
    Asset::Handle fragmentShader = AssetManager::Get("Assets/Shaders/Forward/ForwardFragment.hlsl", AssetType::Shader);

    // Depth buffer
    {
        TextureDesc desc = {};
        desc.Width = 1280;
        desc.Height = 720;
        desc.Levels = 1;
        desc.Depth = 1;
        desc.Name = "Depth Buffer";
        desc.Usage = TextureUsage::DepthTarget;
        desc.Format = TextureFormat::Depth32;
        mDepthBuffer = mRHI->CreateTexture(desc);

        mDepthView = mRHI->CreateView(mDepthBuffer, ViewType::DepthTarget);
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

    // Camera buffer
    {
        for (int i = 0; i < FRAMES_IN_FLIGHT; i++) {
            mCameraBuffer[i] = mRHI->CreateBuffer(256, 0, BufferType::Constant, "Camera Buffer");
            mCameraBuffer[i]->BuildCBV();
        }
    }

    // Sampler
    {
        mSampler = mRHI->CreateSampler(SamplerAddress::Wrap, SamplerFilter::Linear, true);
    }
}

void Forward::Render(const Frame& frame, Scene& scene)
{
    SceneCamera camera = scene.GetMainCamera();
    mCameraBuffer[frame.FrameIndex]->CopyMapped(&camera, sizeof(camera));

    frame.CommandBuffer->BeginMarker("Forward");
    frame.CommandBuffer->Barrier(frame.Backbuffer, ResourceLayout::ColorWrite);
    frame.CommandBuffer->Barrier(mDepthBuffer, ResourceLayout::DepthWrite);
    frame.CommandBuffer->SetViewport(0, 0, frame.Width, frame.Height);
    frame.CommandBuffer->SetRenderTargets({ frame.BackbufferView }, mDepthView);
    frame.CommandBuffer->ClearRenderTarget(frame.BackbufferView, 0.0f, 0.0f, 0.0f);
    frame.CommandBuffer->ClearDepth(mDepthView);
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
                mCameraBuffer[frame.FrameIndex]->CBV(),
                primitive.VertexBuffer->SRV(),
                primitive.IndexBuffer->SRV(),
                primitive.MeshletBuffer->SRV(),
                primitive.MeshletVertices->SRV(),
                primitive.MeshletTriangles->SRV(),
                material.AlbedoView->GetDescriptor().Index,
                mSampler->BindlesssSampler()
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
    auto registry = scene.GetRegistry();
    auto view = registry->view<TransformComponent, MeshComponent>();
    for (auto [entity, transform, mesh]: view.each()) {
        drawNode(frame, mesh.MeshAsset->Mesh.Root, &mesh.MeshAsset->Mesh, transform.Matrix);
    }

    frame.CommandBuffer->Barrier(frame.Backbuffer, ResourceLayout::Shader);
    frame.CommandBuffer->Barrier(mDepthBuffer, ResourceLayout::Common);
    frame.CommandBuffer->EndMarker();
}

void Forward::UI(const Frame& frame)
{

}
