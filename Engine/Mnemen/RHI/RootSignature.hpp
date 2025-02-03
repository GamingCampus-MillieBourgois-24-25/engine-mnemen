//
// > Notice: Amélie Heinrich @ 2024
// > Create Time: 2024-12-03 18:58:51
//

#pragma once

#include <RHI/Device.hpp>

enum class RootType
{
    PushConstant = 0xFFFF,
    ConstantBuffer = D3D12_DESCRIPTOR_RANGE_TYPE_CBV,
    ShaderResource = D3D12_DESCRIPTOR_RANGE_TYPE_SRV,
    Storage = D3D12_DESCRIPTOR_RANGE_TYPE_UAV,
    Sampler = D3D12_DESCRIPTOR_RANGE_TYPE_SAMPLER
};

class RootSignature
{
public:
    using Ref = Ref<RootSignature>;

    RootSignature(Device::Ref device);
    RootSignature(Device::Ref device, const Vector<RootType>& roots, UInt64 pushConstantSize = 0);
    ~RootSignature();

    ID3D12RootSignature* GetSignature() { return mRootSignature; }
private:
    ID3D12RootSignature* mRootSignature = nullptr;
};
