//
// > Notice: Amélie Heinrich @ 2025
// > Create Time: 2025-02-03 22:13:36
//

#pragma once

#include "GraphicsPipeline.hpp"

#include <Agility/d3dx12/d3dx12.h>

class MeshPipeline
{
public:
    using Ref = Ref<MeshPipeline>;

    MeshPipeline(Device::Ref devicePtr, GraphicsPipelineSpecs& specs);
    ~MeshPipeline();

    ID3D12PipelineState* GetPipeline() { return mPipeline; }
    RootSignature::Ref GetSignature() { return mSignature; }
private:
    ID3D12PipelineState* mPipeline;
    RootSignature::Ref mSignature;
};
