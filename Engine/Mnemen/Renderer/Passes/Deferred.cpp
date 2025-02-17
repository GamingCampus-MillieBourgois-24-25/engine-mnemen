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

    Asset::Handle lightShader = AssetManager::Get("Assets/Shaders/Deferred/LightAccumulationCompute.hlsl", AssetType::Shader);

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

    // Accumulation Pipeline
    {
        auto signature = mRHI->CreateRootSignature({ RootType::PushConstant }, sizeof(int) * 4);
        mLightPipeline = mRHI->CreateComputePipeline(lightShader->Shader, signature);
    }
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
