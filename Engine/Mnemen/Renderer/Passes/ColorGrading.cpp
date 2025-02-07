//
// > Notice: Amélie Heinrich @ 2025
// > Create Time: 2025-02-04 12:06:45
//

#include "ColorGrading.hpp"

ColorGrading::ColorGrading(RHI::Ref rhi)
    : RenderPass(rhi)
{
    //retrieve the' computer shader from the asset manager
    Asset::Handle computerShader = AssetManager::Get("Assets/Shaders/ColorGrading/Compute.hlsl", AssetType::Shader); 

    //Create a root signature for the shader (push constants for small data)
    auto signature = mRHI->CreateRootSignature({ RootType::PushConstant }, sizeof(int)* 4 );

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
    }PushConstants = {
        //descriptor of the HDR texture to write to (storage view type)
        color->Descriptor(ViewType::Storage),
        1.0,
        1.0,
        0.0
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
    
}
