//
// > Notice: Amélie Heinrich @ 2024
// > Create Time: 2024-12-18 23:04:48
//

#pragma once

#include <RHI/Device.hpp>
#include <RHI/RootSignature.hpp>

#include <Asset/Shader.hpp>

class ComputePipeline
{
public:
    using Ref = Ref<ComputePipeline>;

    ComputePipeline(Device::Ref device, Shader shader, RootSignature::Ref signature);
    ~ComputePipeline();

    ID3D12PipelineState* GetPipeline() { return mPipeline; }
    RootSignature::Ref GetSignature() { return mSignature; }
private:
    ID3D12PipelineState* mPipeline = nullptr;
    RootSignature::Ref mSignature = nullptr;
};
