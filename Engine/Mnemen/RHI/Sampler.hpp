//
// > Notice: Amélie Heinrich @ 2024
// > Create Time: 2024-12-04 03:20:02
//

#pragma once

#include <RHI/Device.hpp>
#include <RHI/DescriptorHeap.hpp>

enum class SamplerAddress
{
    Wrap = D3D12_TEXTURE_ADDRESS_MODE_WRAP,
    Mirror = D3D12_TEXTURE_ADDRESS_MODE_MIRROR,
    Clamp = D3D12_TEXTURE_ADDRESS_MODE_CLAMP,
    Border = D3D12_TEXTURE_ADDRESS_MODE_BORDER
};

enum class SamplerFilter
{
    Linear = D3D12_FILTER_MIN_MAG_MIP_LINEAR,
    Nearest = D3D12_FILTER_MIN_MAG_MIP_POINT,
    Anistropic = D3D12_FILTER_ANISOTROPIC
};

class Sampler
{
public:
    using Ref = Ref<Sampler>;

    Sampler(Device::Ref device, DescriptorHeaps heaps, SamplerAddress address, SamplerFilter filter, bool mips = false, int anisotropyLevel = 1, bool comparison = false);
    ~Sampler();

    DescriptorHeap::Descriptor GetDescriptor() { return mDescriptor; }

    int BindlesssSampler() { return mDescriptor.Index; }

    SamplerAddress Address() { return mAddress; }
    SamplerFilter Filter() { return mFilter; }
    bool HasMips() { return mMips; }
    int AnisotropyLevel() { return mAnisotropyLevel; } 
private:
    SamplerAddress mAddress;
    SamplerFilter mFilter;
    bool mMips = false;
    int mAnisotropyLevel = 0;

    DescriptorHeap::Descriptor mDescriptor;
    DescriptorHeaps mHeaps;
};
