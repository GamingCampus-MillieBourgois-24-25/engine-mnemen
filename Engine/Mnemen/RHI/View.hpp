//
// > Notice: Amélie Heinrich @ 2024
// > Create Time: 2024-12-03 11:03:43
//

#pragma once

#include <RHI/Resource.hpp>
#include <RHI/DescriptorHeap.hpp>
#include <RHI/Texture.hpp>

enum class ViewType
{
    RenderTarget,
    DepthTarget,
    ShaderResource,
    Storage
};

// TODO(amelie): Add support for TextureArray (for cube maps!!)
enum class ViewDimension
{
    Buffer,
    Texture,
    TextureCube
};

constexpr UInt64 VIEW_ALL_MIPS = 0xFFFFFFFF;

class View
{
public:
    using Ref = Ref<View>;

    View(Device::Ref device, DescriptorHeaps heaps, ::Ref<Resource> resource, ViewType type, ViewDimension dimension = ViewDimension::Texture, TextureFormat format = TextureFormat::Unknown, UInt64 mip = VIEW_ALL_MIPS, UInt64 depthSlice = 0);
    ~View();

    DescriptorHeap::Descriptor GetDescriptor() { return mDescriptor; }
    ::Ref<Resource> GetParentResource() { return mParent; }
    ViewType GetType() { return mType; }
    ViewDimension GetDimension() { return mDimension; }
private:
    DescriptorHeap::Descriptor mDescriptor;
    ::Ref<Resource> mParent = nullptr;
    ViewType mType;
    ViewDimension mDimension;
};
