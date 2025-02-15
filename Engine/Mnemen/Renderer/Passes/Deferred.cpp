//
// > Notice: Amélie Heinrich @ 2025
// > Create Time: 2025-02-03 22:01:27
//

#include "Deferred.hpp"

#include <Asset/AssetManager.hpp>
#include <Core/Application.hpp>
#include <Core/Profiler.hpp>
#include <Core/Statistics.hpp>

Deferred::Deferred(RHI::Ref rhi)
    : RenderPass(rhi)
{
    int width, height;
    Application::Get()->GetWindow()->PollSize(width, height);

    Asset::Handle gbufferShaderIn = AssetManager::Get("Assets/Shaders/Deferred/GBufferMesh.hlsl", AssetType::Shader);
    Asset::Handle gbufferShaderOut = AssetManager::Get("Assets/Shaders/Deferred/GBufferFragment.hlsl", AssetType::Shader);
    Asset::Handle lightShader = AssetManager::Get("Assets/Shaders/Deferred/LightAccumulationCompute.hlsl", AssetType::Shader);

    // Depth buffer
    {
        TextureDesc desc = {};
        desc.Width = width;
        desc.Height = height;
        desc.Levels = 1;
        desc.Depth = 1;
        desc.Name = "GBuffer - Depth";
        desc.Usage = TextureUsage::DepthTarget;
        desc.Format = TextureFormat::Depth32;
       
        auto depthBuffer = RendererTools::CreateSharedTexture("GBufferDepth", desc);
        depthBuffer->AddView(ViewType::DepthTarget);
        depthBuffer->AddView(ViewType::ShaderResource, ViewDimension::Texture, TextureFormat::R32Float);
    }

    // Normal buffer
    {
        TextureDesc desc = {};
        desc.Width = width;
        desc.Height = height;
        desc.Levels = 1;
        desc.Depth = 1;
        desc.Name = "GBuffer - Normals";
        desc.Usage = TextureUsage::RenderTarget | TextureUsage::Storage | TextureUsage::ShaderResource;
        desc.Format = TextureFormat::RGB11Float;
       
        auto renderTarget = RendererTools::CreateSharedTexture("GBufferNormal", desc);
        renderTarget->AddView(ViewType::RenderTarget);
        renderTarget->AddView(ViewType::ShaderResource);
        renderTarget->AddView(ViewType::Storage);
    }

    // Albedo buffer
    {
        TextureDesc desc = {};
        desc.Width = width;
        desc.Height = height;
        desc.Levels = 1;
        desc.Depth = 1;
        desc.Name = "GBuffer - Albedo";
        desc.Usage = TextureUsage::RenderTarget | TextureUsage::Storage | TextureUsage::ShaderResource;
        desc.Format = TextureFormat::RGBA8;
       
        auto renderTarget = RendererTools::CreateSharedTexture("GBufferAlbedo", desc);
        renderTarget->AddView(ViewType::RenderTarget);
        renderTarget->AddView(ViewType::ShaderResource);
        renderTarget->AddView(ViewType::Storage);
    }

    // Color buffer
    {
        TextureDesc desc = {};
        desc.Width = width;
        desc.Height = height;
        desc.Levels = 1;
        desc.Depth = 1;
        desc.Name = "Main Color Buffer";
        desc.Usage = TextureUsage::RenderTarget | TextureUsage::Storage | TextureUsage::ShaderResource;
        desc.Format = TextureFormat::RGBA16Float;
       
        auto renderTarget = RendererTools::CreateSharedTexture("HDRColorBuffer", desc);
        renderTarget->AddView(ViewType::RenderTarget);
        renderTarget->AddView(ViewType::ShaderResource);
        renderTarget->AddView(ViewType::Storage);
    }

    // GBuffer Pipeline
    {
        GraphicsPipelineSpecs specs = {};
        specs.Bytecodes[ShaderType::Mesh] = gbufferShaderIn->Shader;
        specs.Bytecodes[ShaderType::Fragment] = gbufferShaderOut->Shader;
        specs.Formats.push_back(TextureFormat::RGB11Float);
        specs.Formats.push_back(TextureFormat::RGBA8);
        specs.Cull = CullMode::None;
        specs.Fill = FillMode::Solid;
        specs.Depth = DepthOperation::Less;
        specs.DepthEnabled = true;
        specs.DepthFormat = TextureFormat::Depth32;
        specs.CCW = false;
        specs.Signature = mRHI->CreateRootSignature({ RootType::PushConstant }, sizeof(int) * 12 + sizeof(glm::mat4));

        mPipeline = mRHI->CreateMeshPipeline(specs);
    }

    // Accumulation Pipeline
    {
        auto signature = mRHI->CreateRootSignature({ RootType::PushConstant }, sizeof(int) * 4);
        mLightPipeline = mRHI->CreateComputePipeline(lightShader->Shader, signature);
    }

    RendererTools::CreateSharedRingBuffer("CameraRingBuffer", 512);
    RendererTools::CreateSharedSampler("MaterialSampler", SamplerFilter::Linear, SamplerAddress::Wrap, true);
}

void Deferred::Render(const Frame& frame, ::Ref<Scene> scene)
{
    PROFILE_FUNCTION();

    auto cameraBuffer = RendererTools::Get("CameraRingBuffer");
    auto sampler = RendererTools::Get("MaterialSampler");

    auto depthBuffer = RendererTools::Get("GBufferDepth");
    auto normalBuffer = RendererTools::Get("GBufferNormal");
    auto albedoBuffer = RendererTools::Get("GBufferAlbedo");
    auto colorBuffer = RendererTools::Get("HDRColorBuffer");
    auto whiteTexture = RendererTools::Get("WhiteTexture");

    CameraComponent camera = {};
    if (scene) {
        camera = scene->GetMainCamera();
    }
    glm::mat4 matrices[] = {
        camera.View,
        camera.Projection
    };
    cameraBuffer->RBuffer[frame.FrameIndex]->CopyMapped(matrices, sizeof(matrices));

    frame.CommandBuffer->BeginMarker("Deferred");
    // GBuffer
    {
        frame.CommandBuffer->BeginMarker("GBuffer");
        frame.CommandBuffer->Barrier(albedoBuffer->Texture, ResourceLayout::ColorWrite);
        frame.CommandBuffer->Barrier(normalBuffer->Texture, ResourceLayout::ColorWrite);
        frame.CommandBuffer->Barrier(depthBuffer->Texture, ResourceLayout::DepthWrite);
        frame.CommandBuffer->SetViewport(0, 0, frame.Width, frame.Height);
        frame.CommandBuffer->SetRenderTargets({ normalBuffer->GetView(ViewType::RenderTarget), albedoBuffer->GetView(ViewType::RenderTarget) }, depthBuffer->GetView(ViewType::DepthTarget));
        frame.CommandBuffer->ClearRenderTarget(albedoBuffer->GetView(ViewType::RenderTarget), 0.0f, 0.0f, 0.0f);
        frame.CommandBuffer->ClearRenderTarget(normalBuffer->GetView(ViewType::RenderTarget), 0.0f, 0.0f, 0.0f);
        frame.CommandBuffer->ClearDepth(depthBuffer->GetView(ViewType::DepthTarget));
        frame.CommandBuffer->SetMeshPipeline(mPipeline);

        // Draw function for each model
        std::function<void(Frame frame, MeshNode*, Mesh* model, glm::mat4 transform)> drawNode = [&](Frame frame, MeshNode* node, Mesh* model, glm::mat4 transform) {
            if (!node) {
                return;
            }

            glm::mat4 globalTransform = transform * node->Transform;
            for (MeshPrimitive primitive : node->Primitives) {
                Statistics::Get().InstanceCount++;
                MeshMaterial material = model->Materials[primitive.MaterialIndex];

                int albedoIndex = material.Albedo ? material.AlbedoView->GetDescriptor().Index : whiteTexture->Descriptor(ViewType::ShaderResource);

                struct PushConstants {
                    int Matrices;
                    int VertexBuffer;
                    int IndexBuffer;
                    int MeshletBuffer;
                    int MeshletVertices;
                    int MeshletTriangleBuffer;
                    int Albedo;
                    int Sampler;
                    int ShowMeshlets;
                    glm::ivec3 Padding;

                    glm::mat4 Transform;
                } data = {
                    cameraBuffer->Descriptor(ViewType::None, frame.FrameIndex),
                    primitive.VertexBuffer->SRV(),
                    primitive.IndexBuffer->SRV(),
                    primitive.MeshletBuffer->SRV(),
                    primitive.MeshletVertices->SRV(),
                    primitive.MeshletTriangles->SRV(),
                    albedoIndex,
                    sampler->Descriptor(),
                    camera.Volume.VisualizeMeshlets,
                    glm::ivec3(0),
                    
                    transform
                };
                frame.CommandBuffer->GraphicsPushConstants(&data, sizeof(data), 0);
                frame.CommandBuffer->DispatchMesh(primitive.MeshletCount, primitive.IndexCount / 3);
            }
            if (!node->Children.empty()) {
                for (MeshNode* child : node->Children) {
                    drawNode(frame, child, model, globalTransform);
                }
            }
        };

        // Iterate over every mesh. This is messy as hell but fuck it
        if (scene) {
            auto registry = scene->GetRegistry();
            auto view = registry->view<TransformComponent, MeshComponent>();
            for (auto [id, transform, mesh] : view.each()) {
                Entity entity(registry);
                entity.ID = id;

                if (mesh.Loaded) {
                    drawNode(frame, mesh.MeshAsset->Mesh.Root, &mesh.MeshAsset->Mesh, entity.GetWorldTransform());
                }
            }
        }
        frame.CommandBuffer->Barrier(albedoBuffer->Texture, ResourceLayout::Shader);
        frame.CommandBuffer->Barrier(normalBuffer->Texture, ResourceLayout::Shader);
        frame.CommandBuffer->Barrier(depthBuffer->Texture, ResourceLayout::Shader);
        frame.CommandBuffer->EndMarker();
    }
    // Light Accumulation
    {
        struct PushConstants {
            int Depth;
            int Albedo;
            int Normal;
            int Output;
        } data = {
            depthBuffer->Descriptor(ViewType::ShaderResource),
            albedoBuffer->Descriptor(ViewType::ShaderResource),
            normalBuffer->Descriptor(ViewType::ShaderResource),
            colorBuffer->Descriptor(ViewType::Storage)
        };

        frame.CommandBuffer->BeginMarker("Light Accumulation");
        frame.CommandBuffer->Barrier(colorBuffer->Texture, ResourceLayout::Storage);
        frame.CommandBuffer->SetComputePipeline(mLightPipeline);
        frame.CommandBuffer->ComputePushConstants(&data, sizeof(data), 0);
        frame.CommandBuffer->Dispatch(frame.Width / 7, frame.Height / 7, 1);
        frame.CommandBuffer->UAVBarrier(colorBuffer->Texture);
        frame.CommandBuffer->EndMarker();
    }
    frame.CommandBuffer->EndMarker();
}
