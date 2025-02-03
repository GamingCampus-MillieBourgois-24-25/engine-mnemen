//
// > Notice: Amélie Heinrich @ 2024
// > Create Time: 2024-12-04 03:21:22
//

#include <RHI/Sampler.hpp>

Sampler::Sampler(Device::Ref device, DescriptorHeaps heaps, SamplerAddress address, SamplerFilter filter, bool mips, int anisotropyLevel, bool comparison)
    : mHeaps(heaps), mAddress(address), mFilter(filter), mMips(mips), mAnisotropyLevel(anisotropyLevel)
{
    D3D12_SAMPLER_DESC samplerDesc = {};
    samplerDesc.AddressU = D3D12_TEXTURE_ADDRESS_MODE(address);
    samplerDesc.AddressV = samplerDesc.AddressU;
    samplerDesc.AddressW = samplerDesc.AddressV;
    samplerDesc.Filter = D3D12_FILTER(filter);
    samplerDesc.MaxAnisotropy = anisotropyLevel;
    samplerDesc.ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
    samplerDesc.MinLOD = 0.0f;
    if (mips) {
        samplerDesc.MaxLOD = D3D12_FLOAT32_MAX;
    }
    samplerDesc.MipLODBias = 0.0f;
    samplerDesc.BorderColor[0] = 1.0f;
    samplerDesc.BorderColor[1] = 1.0f;
    samplerDesc.BorderColor[2] = 1.0f;
    samplerDesc.BorderColor[3] = 1.0f;
    if (comparison) {
        samplerDesc.Filter = D3D12_FILTER_COMPARISON_MIN_MAG_LINEAR_MIP_POINT;
        samplerDesc.ComparisonFunc = D3D12_COMPARISON_FUNC_LESS_EQUAL;
    }

    mDescriptor = heaps[DescriptorHeapType::Sampler]->Allocate();
    device->GetDevice()->CreateSampler(&samplerDesc, mDescriptor.CPU);
}

Sampler::~Sampler()
{
    mDescriptor.Parent->Free(mDescriptor);
}
