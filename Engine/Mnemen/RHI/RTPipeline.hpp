//
// > Notice: Amélie Heinrich @ 2024
// > Create Time: 2024-12-18 22:48:18
//

#pragma once

#include <Asset/Shader.hpp>
#include <RHI/RootSignature.hpp>
#include <RHI/Buffer.hpp>

struct RTPipelineSpecs
{
    UInt32 PayloadSize;
    UInt32 AttribSize = 8;
    UInt32 MaxRecursion = 3;

    Shader Library;
    RootSignature::Ref Signature = nullptr;
};

class RTPipeline
{
public:
    using Ref = Ref<RTPipeline>;

    RTPipeline(Device::Ref device, DescriptorHeaps& heaps, RTPipelineSpecs specs);
    ~RTPipeline();

    ID3D12StateObject* GetPipeline() { return mPipeline; }
    RootSignature::Ref GetSignature() { return mSignature; }
    Buffer::Ref GetTables() { return mIDBuffer; }
private:
    ID3D12StateObject* mPipeline = nullptr;
    RootSignature::Ref mSignature = nullptr;
    Buffer::Ref mIDBuffer = nullptr;
};
