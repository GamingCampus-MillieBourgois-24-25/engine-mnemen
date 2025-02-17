//
// > Notice: Amélie Heinrich @ 2025
// > Create Time: 2025-02-04 12:06:45
//

#include "SSAO.hpp"
#include "RHI/Uploader.hpp"
#include <Core/Application.hpp>
#include <random>
#include <imgui.h>

float lerp(float a, float b, float f)
{
    return a + f * (b - a);
}

SSAO::SSAO(RHI::Ref rhi)
    : RenderPass(rhi)
{
    Asset::Handle computeShader = AssetManager::Get("Assets/Shaders/SSAO/Compute.hlsl", AssetType::Shader);

    auto signature = mRHI->CreateRootSignature({ RootType::PushConstant }, sizeof(int) * 4 + sizeof(glm::mat4));

    mPipeline = mRHI->CreateComputePipeline(computeShader->Shader, signature);

    int width, height;
    Application::Get()->GetWindow()->PollSize(width, height);

    TextureDesc text2 = {};
    text2.Width = width;
    text2.Height = height;   
    text2.Levels = 1;
    text2.Depth = 1;
    text2.Name = "ssaoColorBuffer";
    text2.Usage = TextureUsage::RenderTarget | TextureUsage::Storage | TextureUsage::ShaderResource;
    text2.Format = TextureFormat::R32Float;
    auto scb = RendererTools::CreateSharedTexture("ssaoColorBuffer", text2);
    scb->AddView(ViewType::Storage);
    scb->AddView(ViewType::ShaderResource);
}

void SSAO::Render(const Frame& frame, ::Ref<Scene> scene)
{
    // Retrieve the HDR color texture that we will write to
    auto depth = RendererTools::Get("GBufferDepth");
    auto color = RendererTools::Get("ssaoColorBuffer");

    CameraComponent* camera = {};
    if (scene) {
        camera = scene->GetMainCamera();
    }

    // Create the push constants structure for settings passed to the shader
    struct {
        int DepthOutput;           // The output texture we will write to (storage view)
        int OutputIndex;
        glm::vec2 Pad;       // Padding for alignment purposes (16-byte boundary)
        glm::mat4 InverseProjection;
    } PushConstants = {
        // Descriptor of the HDR texture to write to (storage view type)
        depth->Descriptor(ViewType::ShaderResource),
        color->Descriptor(ViewType::Storage),
        glm::vec2(0),         // Padding vector,
        glm::inverse(camera->Projection)
    };
    
    // Begin a command marker for easier GPU debugging
    frame.CommandBuffer->BeginMarker("SSAO");

    // Transition the texture layout to notify the GPU that we are going to write to it
    frame.CommandBuffer->Barrier(color->Texture, ResourceLayout::Storage);
    frame.CommandBuffer->Barrier(depth->Texture, ResourceLayout::Shader);

    // Set the compute pipeline for this rendering pass
    frame.CommandBuffer->SetComputePipeline(mPipeline);

    // Push the constants (settings) to the GPU
    frame.CommandBuffer->ComputePushConstants(&PushConstants, sizeof(PushConstants), 0);

    // Dispatch the compute shader (divide the image width and height by 8 since we're using 8x8 thread groups)
    frame.CommandBuffer->Dispatch(frame.Width / 8, frame.Height / 8, 1);

    // After the dispatch, change the texture layout back to 'Common' to indicate we're done writing
    frame.CommandBuffer->Barrier(color->Texture, ResourceLayout::Common);
    frame.CommandBuffer->Barrier(depth->Texture, ResourceLayout::Common);

    // End the command marker
    frame.CommandBuffer->EndMarker();
}