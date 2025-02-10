//
// > Notice: Amélie Heinrich @ 2025
// > Create Time: 2025-02-04 12:06:45
//

#include "DOF.hpp"

DOF::DOF(RHI::Ref rhi)
    : RenderPass(rhi)
{
    Asset::Handle computeShader  = AssetManager::Get("Assets/Shaders/DOF/Compute.hlsl", AssetType::Shader);

    auto signature = mRHI->CreateRootSignature({ RootType::PushConstant}, sizeof(int) * 6);
    mPipeline = mRHI->CreateComputePipeline(computeShader->Shader, signature);
}

void DOF::Render(const Frame& frame, ::Ref<Scene> scene)
{
    auto color = RendererTools::Get("HDRColorBuffer");
    auto depth = RendererTools::Get("GBufferDepth");

    struct {
        int DepthIndex;
        int InputIndex;

        float Far;
        float Golden_Angle;
        float Max_Blur_Size;
        float Rad_Scale;
    } PushConstants = {
        color->Descriptor(ViewType::Storage),
        depth->Descriptor(ViewType::ShaderResource),
        200.0f,
        0.0f
    };

    frame.CommandBuffer->BeginMarker("Box Blur");
    frame.CommandBuffer->Barrier(color->Texture, ResourceLayout::Storage);
    frame.CommandBuffer->Barrier(depth->Texture, ResourceLayout::Shader);
    frame.CommandBuffer->SetComputePipeline(mPipeline);
    frame.CommandBuffer->ComputePushConstants(&PushConstants, sizeof(PushConstants), 0);
    frame.CommandBuffer->Dispatch(frame.Width / 8, frame.Height / 8, 1);
    frame.CommandBuffer->Barrier(color->Texture, ResourceLayout::Common);
    frame.CommandBuffer->Barrier(depth->Texture, ResourceLayout::Common);
    frame.CommandBuffer->EndMarker();
}

void DOF::UI(const Frame& frame)
{
    // Axel
}
