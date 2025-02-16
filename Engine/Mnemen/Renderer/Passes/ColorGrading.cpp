//
// > Notice: Amélie Heinrich @ 2025
// > Create Time: 2025-02-04 12:06:45
//

#include "ColorGrading.hpp"

#include <imgui.h>

ColorGrading::ColorGrading(RHI::Ref rhi)
    : RenderPass(rhi)
{
    //retrieve the' computer shader from the asset manager
    Asset::Handle computerShader = AssetManager::Get("Assets/Shaders/ColorGrading/Compute.hlsl", AssetType::Shader); 

    //Create a root signature for the shader (push constants for small data)
    auto signature = mRHI->CreateRootSignature({ RootType::PushConstant }, sizeof(int) * 28);

    //Create the compute pipeline with the shader and root signature 
    mPipeline = mRHI->CreateComputePipeline(computerShader->Shader, signature);
}


void ColorGrading::Render(const Frame& frame, ::Ref<Scene> scene)
{
    CameraComponent* mainCamera = scene->GetMainCamera();

    // Retrieve the HDR color texture that we will write to 
    auto color = RendererTools::Get("HDRColorBuffer");

    // Create the push constants structure for settings passed to the shader
    struct {
        // float4
        int TextureIndex; 
        float Brightness;   
        float Exposure;
        float Pad;

        // float4
        float Contrast;
        float Saturation;
        glm::vec2 Pad1;

        // float4
        float HueShift;
        float Balance;
        glm::vec2 Pad2;

        // float4
        glm::vec4 Shadows;

        // float4
        glm::vec4 ColorFilter;
        
        // float4
        glm::vec4 HightLights;

        // float4
        float Temparature; 
        float Tint; 
        glm::vec2 Pad3;
    } PushConstants = {
        //descriptor of the HDR texture to write to (storage view type)
        color->Descriptor(ViewType::Storage),
        mainCamera->Volume->Volume.Brightness,
        mainCamera->Volume->Volume.Exposure,
        0.0,

        mainCamera->Volume->Volume.Contrast,
        mainCamera->Volume->Volume.Saturation,
        glm::vec2(0.0f),
       
        mainCamera->Volume->Volume.HueShift,
        mainCamera->Volume->Volume.Balance,
        glm::vec2(0.0f),

        mainCamera->Volume->Volume.Shadows,
        mainCamera->Volume->Volume.ColorFilter,

        mainCamera->Volume->Volume.Highlights,

        mainCamera->Volume->Volume.Temperature,
        mainCamera->Volume->Volume.Tint,
        glm::vec2(0.0f)
    };

    if (mainCamera->Volume->Volume.EnableColorGrading) {
        frame.CommandBuffer->BeginMarker("Color Grading");
        frame.CommandBuffer->Barrier(color->Texture, ResourceLayout::Storage);
        frame.CommandBuffer->SetComputePipeline(mPipeline);
        frame.CommandBuffer->ComputePushConstants(&PushConstants, sizeof (PushConstants), 0);
        frame.CommandBuffer->Dispatch(frame.Width / 8, frame.Height / 8, 1);
        frame.CommandBuffer->Barrier(color->Texture, ResourceLayout::Common);
        frame.CommandBuffer->EndMarker();
    }
}
