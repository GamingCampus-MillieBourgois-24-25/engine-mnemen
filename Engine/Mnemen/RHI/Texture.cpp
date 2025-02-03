//
// > Notice: Amélie Heinrich @ 2024
// > Create Time: 2024-12-03 10:52:32
//

#include <RHI/Texture.hpp>
#include <Core/UTF.hpp>

Texture::Texture(Device::Ref device, ID3D12Resource* resource, TextureDesc desc)
    : Resource(device), mDesc(desc)
{
    mShouldFree = false;
    mResource = resource;
}

Texture::Texture(Device::Ref device, TextureDesc desc)
    : Resource(device), mDesc(desc)
{
    mShouldFree = true;

    D3D12_HEAP_PROPERTIES heapProperties = {};
    heapProperties.Type = D3D12_HEAP_TYPE_DEFAULT;
    
    D3D12_RESOURCE_DESC resourceDesc = {};
    resourceDesc.Width = desc.Width;
    resourceDesc.Height = desc.Height;
    resourceDesc.DepthOrArraySize = desc.Depth;
    resourceDesc.Format = DXGI_FORMAT(desc.Format);
    resourceDesc.MipLevels = desc.Levels;
    resourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
    resourceDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
    resourceDesc.SampleDesc.Count = 1;
    resourceDesc.SampleDesc.Quality = 0;
    resourceDesc.Alignment = D3D12_DEFAULT_RESOURCE_PLACEMENT_ALIGNMENT;

    if (desc.Usage & TextureUsage::RenderTarget)
        resourceDesc.Flags |= D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET;
    if (desc.Usage & TextureUsage::Storage)
        resourceDesc.Flags |= D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS;
    if (desc.Usage & TextureUsage::DepthTarget)
        resourceDesc.Flags |= D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;
    
    CreateResource(&heapProperties, &resourceDesc, D3D12_RESOURCE_STATE_COMMON);
    SetName(desc.Name);
}

Texture::~Texture()
{
    // Everything will automatically be cleaned up by Resource::~Resource().
}

TextureFormat Texture::StringToFormat(const String& format)
{
    if (format == "RGBA8")
        return TextureFormat::RGBA8;
    if (format == "D32")
        return TextureFormat::Depth32;
    if (format == "R32Float")
        return TextureFormat::R32Float;
    if (format == "RGBA16")
        return TextureFormat::RGBA16Float;
    if (format == "RGB11")
        return TextureFormat::RGB11Float;
    if (format == "RG8")
        return TextureFormat::RG8;
    if (format == "R8")
        return TextureFormat::R8;
    return TextureFormat::Unknown;
}
