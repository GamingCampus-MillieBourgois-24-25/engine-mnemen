//
// > Notice: Amélie Heinrich @ 2024
// > Create Time: 2024-12-03 23:10:09
//

#pragma once

#include <RHI/Device.hpp>
#include <RHI/DescriptorHeap.hpp>
#include <RHI/Resource.hpp>

enum class BufferType
{
    Vertex,
    Index,
    Constant,
    Storage,
    Copy,
    Readback,
    AccelerationStructure
};

class Buffer : public Resource
{
public:
    using Ref = Ref<Buffer>;

    Buffer(Device::Ref device, DescriptorHeaps heaps, UInt64 size, UInt64 stride, BufferType type, const String& name = "Buffer");
    ~Buffer();

    void BuildCBV();
    void BuildUAV();
    void BuildSRV();

    void Map(int start, int end, void **data);
    void Unmap(int start, int end);
    void CopyMapped(void *data, UInt64 size);

    Int32 CBV() { return mCBV.Index; }
    Int32 UAV() { return mUAV.Index; }
    Int32 SRV() { return mSRV.Index; }
private:
    friend class CommandBuffer;

    DescriptorHeaps mHeaps;
    BufferType mType;

    DescriptorHeap::Descriptor mCBV = {};
    DescriptorHeap::Descriptor mSRV = {};
    DescriptorHeap::Descriptor mUAV = {};

    D3D12_VERTEX_BUFFER_VIEW mVBV;
    D3D12_INDEX_BUFFER_VIEW mIBV;
};
