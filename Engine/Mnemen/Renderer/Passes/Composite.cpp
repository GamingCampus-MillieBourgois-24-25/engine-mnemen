//
// > Notice: Amélie Heinrich @ 2024
// > Create Time: 2024-12-06 00:22:01
//

#include "Composite.hpp"

#include <glm/gtc/type_ptr.hpp>
#include <ImGui/imgui.h>
#include <Core/Application.hpp>
#include <Core/Profiler.hpp>

Composite::Composite(RHI::Ref rhi)
    : RenderPass(rhi)
{
    int width, height;
    Application::Get()->GetWindow()->PollSize(width, height);

    Asset::Handle computeShader = AssetManager::Get("Assets/Shaders/Composite/Compute.hlsl", AssetType::Shader);
 
    mSignature = mRHI->CreateRootSignature({ RootType::PushConstant }, sizeof(int) * 4);
    mPipeline = mRHI->CreateComputePipeline(computeShader->Shader, mSignature);

    TextureDesc desc = {};
    desc.Width = width;
    desc.Height = height;
    desc.Levels = 1;
    desc.Depth = 1;
    desc.Name = "LDR Color Buffer";
    desc.Usage = TextureUsage::RenderTarget | TextureUsage::Storage | TextureUsage::ShaderResource;
    desc.Format = TextureFormat::RGBA8;
    
    auto ldr = RendererTools::CreateSharedTexture("LDRColorBuffer", desc);
    ldr->AddView(ViewType::Storage);
}

void Composite::Render(const Frame& frame, ::Ref<Scene> scene)
{
    PROFILE_FUNCTION();

    auto hdr = RendererTools::Get("HDRColorBuffer");
    auto ldr = RendererTools::Get("LDRColorBuffer");

    frame.CommandBuffer->BeginMarker("Composite");
    
    struct {
        int Input;
        int Output;
        float Gamma;
        int Pad;
    } PushConstants = {
        hdr->Descriptor(ViewType::ShaderResource),
        ldr->Descriptor(ViewType::Storage),
        2.2,
        0
    };

    // Tonemap color buffer
    frame.CommandBuffer->BeginMarker("Tonemap");
    frame.CommandBuffer->Barrier(hdr->Texture, ResourceLayout::Storage);
    frame.CommandBuffer->Barrier(ldr->Texture, ResourceLayout::Storage);
    frame.CommandBuffer->SetComputePipeline(mPipeline);
    frame.CommandBuffer->ComputePushConstants(&PushConstants, sizeof(PushConstants), 0);
    frame.CommandBuffer->Dispatch(frame.Width / 8, frame.Height / 8, 1);
    frame.CommandBuffer->Barrier(hdr->Texture, ResourceLayout::Common);
    frame.CommandBuffer->UAVBarrier(ldr->Texture);
    frame.CommandBuffer->EndMarker();
    //
    
    // Copy LDR to backbuffer
    frame.CommandBuffer->BeginMarker("Copy to Backbuffer");
    frame.CommandBuffer->Barrier(ldr->Texture, ResourceLayout::CopySource);
    frame.CommandBuffer->Barrier(frame.Backbuffer, ResourceLayout::CopyDest);
    frame.CommandBuffer->CopyTextureToTexture(frame.Backbuffer, ldr->Texture);
    frame.CommandBuffer->Barrier(ldr->Texture, ResourceLayout::Common);
    frame.CommandBuffer->Barrier(frame.Backbuffer, ResourceLayout::ColorWrite);
    frame.CommandBuffer->EndMarker();
    //

    frame.CommandBuffer->EndMarker();
}

void Composite::UI(const Frame& frame)
{

}
