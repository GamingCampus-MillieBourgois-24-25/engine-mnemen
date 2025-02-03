//
// > Notice: Amélie Heinrich @ 2024
// > Create Time: 2024-12-03 09:32:16
//

#pragma once

#include <RHI/Device.hpp>

enum class DescriptorHeapType
{
    ShaderResource = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV,
    Sampler = D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER,
    RenderTarget = D3D12_DESCRIPTOR_HEAP_TYPE_RTV,
    DepthTarget = D3D12_DESCRIPTOR_HEAP_TYPE_DSV
};

class DescriptorHeap
{
public:
    using Ref = Ref<DescriptorHeap>;

    struct Descriptor
    {
        bool Valid = false;
        int Index = -1;
        DescriptorHeap* Parent = nullptr;

        D3D12_CPU_DESCRIPTOR_HANDLE CPU;
        D3D12_GPU_DESCRIPTOR_HANDLE GPU;
        
        Descriptor() = default;
        Descriptor(DescriptorHeap* heap, int index)
            : Parent(heap), Index(index), Valid(true)
        {
            CPU = Parent->mHeap->GetCPUDescriptorHandleForHeapStart();
            CPU.ptr += index * Parent->mIncrementSize;

            if (Parent->mShaderVisible) {
                GPU = Parent->mHeap->GetGPUDescriptorHandleForHeapStart();
                GPU.ptr += index * Parent->mIncrementSize;
            }
        }
    };

    DescriptorHeap(Device::Ref device, DescriptorHeapType type, UInt32 size);
    ~DescriptorHeap();

    Descriptor Allocate();
    void Free(Descriptor& descriptor);

    ID3D12DescriptorHeap* GetHeap() { return mHeap; }
private:
    DescriptorHeapType mType;
    ID3D12DescriptorHeap* mHeap = nullptr;

    int mIncrementSize = 0;
    UInt32 mHeapSize = 0;
    bool mShaderVisible = false;
    Vector<bool> mLookupTable;
};

using DescriptorHeaps = UnorderedMap<DescriptorHeapType, DescriptorHeap::Ref>;
