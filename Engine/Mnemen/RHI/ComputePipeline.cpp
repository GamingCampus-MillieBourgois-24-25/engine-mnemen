//
// > Notice: Amélie Heinrich @ 2024
// > Create Time: 2024-12-18 23:39:30
//

#include <RHI/ComputePipeline.hpp>
#include <Core/Logger.hpp>

ComputePipeline::ComputePipeline(Device::Ref device, Shader shader, RootSignature::Ref signature)
    : mSignature(signature)
{
    D3D12_COMPUTE_PIPELINE_STATE_DESC desc = {};
    desc.CS.pShaderBytecode = shader.Bytecode.data();
    desc.CS.BytecodeLength = shader.Bytecode.size();
    desc.pRootSignature = signature->GetSignature();

    HRESULT Result = device->GetDevice()->CreateComputePipelineState(&desc, IID_PPV_ARGS(&mPipeline));
    if (FAILED(Result)) {
        LOG_CRITICAL("Failed creating D3D12 compute pipeline!");
        return;
    }
}

ComputePipeline::~ComputePipeline()
{
    mPipeline->Release();
}
