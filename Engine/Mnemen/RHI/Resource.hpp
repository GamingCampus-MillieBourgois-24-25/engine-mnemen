//
// > Notice: Amélie Heinrich @ 2024
// > Create Time: 2024-12-03 10:38:04
//

#pragma once

#include <RHI/Device.hpp>

// We go through a parent Resource class so it's easier to tag/track them later down the line.

enum class ResourceLayout
{
    Common = D3D12_RESOURCE_STATE_COMMON,
    Shader = D3D12_RESOURCE_STATE_ALL_SHADER_RESOURCE,
    Storage = D3D12_RESOURCE_STATE_UNORDERED_ACCESS,
    DepthWrite = D3D12_RESOURCE_STATE_DEPTH_WRITE,
    DepthRead = D3D12_RESOURCE_STATE_DEPTH_READ,
    ColorWrite = D3D12_RESOURCE_STATE_RENDER_TARGET,
    CopySource = D3D12_RESOURCE_STATE_COPY_SOURCE,
    CopyDest = D3D12_RESOURCE_STATE_COPY_DEST,
    Present = D3D12_RESOURCE_STATE_PRESENT,
    GenericRead = D3D12_RESOURCE_STATE_GENERIC_READ,
    Vertex = D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER,
    AccelerationStructure = D3D12_RESOURCE_STATE_RAYTRACING_ACCELERATION_STRUCTURE,
    NonPixelShader = D3D12_RESOURCE_STATE_NON_PIXEL_SHADER_RESOURCE
};

enum class ResourceTag
{
    ModelGeometry,
    ModelTexture,
    ShaderPassIO,
    ShaderPassResource,
    GPUReadback
};

class Resource
{
public:
    Resource(Device::Ref device);
    ~Resource();

    void SetName(const String& string);
    void Tag(ResourceTag tag);

    UInt64 GetSize() const { return mSize; }
    UInt64 GetStride() const { return mStride; }
    ID3D12Resource* GetResource() const { return mResource; }
    UInt64 GetAddress() { return mResource->GetGPUVirtualAddress(); }
    String GetName() { return mName; }

    ResourceLayout GetLayout() { return mLayout; };
    void SetLayout(ResourceLayout layout) { mLayout = layout; }
protected:
    Device::Ref mParentDevice;
    ID3D12Resource* mResource = nullptr;
    
    bool mShouldFree = false;
    UInt64 mSize;
    UInt64 mStride;
    ResourceLayout mLayout;
    String mName;

    Vector<ResourceTag> mTags;

    void CreateResource(D3D12_HEAP_PROPERTIES* heapProps, D3D12_RESOURCE_DESC* resourceDesc, D3D12_RESOURCE_STATES state);
private:
    UInt64 mAllocSize;
};
