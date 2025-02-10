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
    auto signature = mRHI->CreateRootSignature({ RootType::PushConstant }, sizeof(int) * 24);

    //Create the compute pipeline with the shader and root signature 
    mPipeline = mRHI->CreateComputePipeline(computerShader->Shader, signature);

}


void ColorGrading::Render(const Frame& frame, ::Ref<Scene> scene)
{
    //Retrieve the HDR color texture that we will write to 
    auto color = RendererTools::Get("HDRColorBuffer");

    //Create the push constants structure for settings passed to the shader
    struct{
        int TextureIndex; 
        float Brightness;   
        float Exposure;
        float Pad;

        float Contrast;
        float Saturation;
        glm::vec2 Pad1;

        float HueShift;
        float Balance;
        glm::vec2 Pad2;

        glm::vec4 Shadows;
        glm::vec4 ColorFilter;
        
        glm::vec4 HightLights;

        float Temparature; 
        float Tint; 
        glm::vec2 Pad3;
    } PushConstants = {
        //descriptor of the HDR texture to write to (storage view type)
        color->Descriptor(ViewType::Storage),
        mBrightness,
        mExposure,
        0.0,

        mContrast,
        mSaturation,
        glm::vec2(0.0f),
       
        mHueShift,
        mBalance,
        glm::vec2(0.0f),

        mShadows, 
        mColorFilter,

        mHightLigths,

        mTemperature,
        mTint,
        glm::vec2(0.0f)
    };

    // -> command marker for esaier GPU debugging
    frame.CommandBuffer->BeginMarker("Color Grading");

    // Transition the texture to notify the GPU (we are going to write to it )
    frame.CommandBuffer->Barrier(color->Texture, ResourceLayout::Storage);

    // Set the compute pipeline for this rendering pass
    frame.CommandBuffer->SetComputePipeline(mPipeline);

    // Push the constants (settings) to the GPU
    frame.CommandBuffer->ComputePushConstants(&PushConstants, sizeof (PushConstants), 0);

    // Dispatch the compute shader (divide the image width and height by 8 since we're using 8x8 thread groups)
    frame.CommandBuffer->Dispatch(frame.Width / 8, frame.Height / 8, 1);

    // After the dispatch, change the texture layout back to 'Common' to indicate we're done writing
    frame.CommandBuffer->Barrier(color->Texture, ResourceLayout::Common);

    //End the command marker
    frame.CommandBuffer->EndMarker();
}

void ColorGrading::UI(const Frame& frame)
{
    if (ImGui::TreeNodeEx("Color Grading", ImGuiTreeNodeFlags_Framed)) {
        ImGui::SliderFloat("Brightness", &mBrightness, 0.0f, 10.0f, "%.2f");
        ImGui::SliderFloat("Exposure", &mExposure, 0.0f, 10.0f, "%.2f");
        ImGui::SliderFloat("Saturation", &mSaturation, -10.0f, 10.0f, "%.2f");
        ImGui::SliderFloat("Contrast", &mContrast, -10.0f, 10.0f, "%.2f");
        ImGui::SliderFloat("Hue Shift", &mHueShift, -180.0f, 180.0f, "%.1f");
        ImGui::SliderFloat("Temperature", &mTemperature, -1.0f, 1.0f, "%.1f");
        ImGui::SliderFloat("Tint", &mTint, -1.0f, 1.0f, "%.1f");
        if(ImGui::TreeNodeEx("Split Toning", ImGuiTreeNodeFlags_Framed)){
            ImGui::ColorPicker3("Shadows", glm::value_ptr(mShadows), ImGuiColorEditFlags_PickerHueBar);
            ImGui::ColorPicker3("Hightlights", glm::value_ptr(mHightLigths), ImGuiColorEditFlags_PickerHueBar);
            ImGui::SliderFloat("Balance", &mBalance, -100.0f, 100.0f, "%.1f");
            ImGui::TreePop();
        }
        ImGui::ColorPicker3("Color Filter", glm::value_ptr(mColorFilter), ImGuiColorEditFlags_PickerHueBar);
        ImGui::TreePop();
    }  
}
