//
// > Notice: Amélie Heinrich @ 2024
// > Create Time: 2024-12-03 23:20:17
//

#include <RHI/Buffer.hpp>
#include <RHI/Utilities.hpp>
#include <Core/Assert.hpp>
#include <Core/Logger.hpp>

Buffer::Buffer(Device::Ref device, DescriptorHeaps heaps, UInt64 size, UInt64 stride, BufferType type, const String& name)
    : Resource(device), mType(type), mHeaps(heaps)
{
    mShouldFree = true;
    mSize = size;
    mStride = stride;

    D3D12_HEAP_PROPERTIES heapProperties = {};
    switch (type) {
        case BufferType::Readback:
            heapProperties.Type = D3D12_HEAP_TYPE_READBACK;
            break;
        case BufferType::Constant:
        case BufferType::Copy:
            heapProperties.Type = D3D12_HEAP_TYPE_UPLOAD;
            break;
        default:
            heapProperties.Type = D3D12_HEAP_TYPE_DEFAULT;
            break;
    }

    D3D12_RESOURCE_DESC resourceDesc = {};
    resourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
    resourceDesc.Alignment = D3D12_DEFAULT_RESOURCE_PLACEMENT_ALIGNMENT;
    resourceDesc.Width = size;
    resourceDesc.Height = 1;
    resourceDesc.DepthOrArraySize = 1;
    resourceDesc.MipLevels = 1;
    resourceDesc.Format = DXGI_FORMAT_UNKNOWN;
    resourceDesc.SampleDesc.Count = 1;
    resourceDesc.SampleDesc.Quality = 0;
    resourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
    resourceDesc.Flags = D3D12_RESOURCE_FLAG_NONE;
    if (type == BufferType::Storage || type == BufferType::AccelerationStructure) resourceDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS | D3D12_RESOURCE_FLAG_RAYTRACING_ACCELERATION_STRUCTURE;
    if (type == BufferType::Constant) mLayout = ResourceLayout::GenericRead;
    if (type == BufferType::AccelerationStructure) mLayout = ResourceLayout::AccelerationStructure;

    CreateResource(&heapProperties, &resourceDesc, D3D12_RESOURCE_STATES(mLayout));
    SetName(name);

    switch (type) {
        case BufferType::Vertex: {
            mVBV.BufferLocation = GetAddress();
            mVBV.SizeInBytes = size;
            mVBV.StrideInBytes = stride;
            break;
        }
        case BufferType::Index: {
            mIBV.BufferLocation = GetAddress();
            mIBV.SizeInBytes = size;
            mIBV.Format = DXGI_FORMAT_R32_UINT;
            break;
        }
    }
}

Buffer::~Buffer()
{
    mHeaps[DescriptorHeapType::ShaderResource]->Free(mCBV);
    mHeaps[DescriptorHeapType::ShaderResource]->Free(mUAV);
    mHeaps[DescriptorHeapType::ShaderResource]->Free(mSRV);
}

void Buffer::BuildCBV()
{
    D3D12_CONSTANT_BUFFER_VIEW_DESC cbvd = {};
    cbvd.BufferLocation = GetAddress();
    cbvd.SizeInBytes = mSize;
    if (mCBV.Valid == false)
        mCBV = mHeaps[DescriptorHeapType::ShaderResource]->Allocate();
    mParentDevice->GetDevice()->CreateConstantBufferView(&cbvd, mCBV.CPU);
}

void Buffer::BuildUAV()
{
    D3D12_UNORDERED_ACCESS_VIEW_DESC uavd = {};
    uavd.ViewDimension = D3D12_UAV_DIMENSION_BUFFER;
    uavd.Format = DXGI_FORMAT_UNKNOWN;
    uavd.Buffer.Flags = D3D12_BUFFER_UAV_FLAG_NONE;
    uavd.Buffer.FirstElement = 0;
    uavd.Buffer.NumElements = mSize / 4;
    uavd.Buffer.StructureByteStride = 4;
    uavd.Buffer.CounterOffsetInBytes = 0;
    if (mUAV.Valid == false)
        mUAV = mHeaps[DescriptorHeapType::ShaderResource]->Allocate();
    mParentDevice->GetDevice()->CreateUnorderedAccessView(mResource, nullptr, &uavd, mUAV.CPU);
}

void Buffer::BuildSRV()
{
    D3D12_SHADER_RESOURCE_VIEW_DESC srv = {};
    srv.ViewDimension = D3D12_SRV_DIMENSION::D3D12_SRV_DIMENSION_BUFFER;
    srv.Format = DXGI_FORMAT_UNKNOWN;
    srv.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
    srv.Buffer.Flags = D3D12_BUFFER_SRV_FLAG_NONE;
    srv.Buffer.FirstElement = 0;
    srv.Buffer.NumElements = mSize / mStride;
    srv.Buffer.StructureByteStride = mStride;
    if (mSRV.Valid == false)
        mSRV = mHeaps[DescriptorHeapType::ShaderResource]->Allocate();
    mParentDevice->GetDevice()->CreateShaderResourceView(mResource, &srv, mSRV.CPU);
}

void Buffer::Map(int start, int end, void **data)
{
    D3D12_RANGE range;
    range.Begin = start;
    range.End = end;

    if (range.End > range.Begin) {
        if (FAILED(mResource->Map(0, &range, data))) {
            LOG_ERROR("Failed to map buffer!");
        }
    } else {
        if (FAILED(mResource->Map(0, nullptr, data))) {
            LOG_ERROR("Failed to map buffer!");
        }
    }
}

void Buffer::Unmap(int start, int end)
{
    D3D12_RANGE range;
    range.Begin = start;
    range.End = end;

    if (range.End > range.Begin) {
        mResource->Unmap(0, &range);
    } else {
        mResource->Unmap(0, nullptr);
    }
}

void Buffer::CopyMapped(void *data, UInt64 size)
{
    void* mapped = nullptr;
    Map(0, 0, &mapped);
    if (mapped) {
        memcpy(mapped, data, size);
    }
    Unmap(0, 0);
}
