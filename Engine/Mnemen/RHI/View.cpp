//
// > Notice: Amélie Heinrich @ 2024
// > Create Time: 2024-12-03 11:10:28
//

#include <RHI/View.hpp>
#include <Core/Assert.hpp>

View::View(Device::Ref device, DescriptorHeaps heaps, ::Ref<Resource> resource, ViewType type, ViewDimension dimension, TextureFormat format, UInt64 mip, UInt64 depthSlice)
    : mParent(resource), mType(type), mDimension(dimension)
{
    // This is a bit of a mess but it is what it is. Sorry!

    switch (type)
    {
    case ViewType::ShaderResource: {
        mDescriptor = heaps[DescriptorHeapType::ShaderResource]->Allocate();

        D3D12_SHADER_RESOURCE_VIEW_DESC desc = {};
        desc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
        desc.ViewDimension = dimension == ViewDimension::Buffer ? D3D12_SRV_DIMENSION_BUFFER : D3D12_SRV_DIMENSION_TEXTURE2D;
        desc.Format = DXGI_FORMAT_UNKNOWN;
        if (dimension == ViewDimension::Texture) {
            TextureDesc texture = static_cast<Texture*>(resource.get())->GetDesc();
            desc.Format = format == TextureFormat::Unknown ? DXGI_FORMAT(texture.Format) : DXGI_FORMAT(format);
            desc.Texture2D.MipLevels = mip == VIEW_ALL_MIPS ? texture.Levels : 1;
            desc.Texture2D.MostDetailedMip = mip == VIEW_ALL_MIPS ? 0 : mip;
        } else if (dimension == ViewDimension::TextureCube) {
            TextureDesc texture = static_cast<Texture*>(resource.get())->GetDesc();
            desc.Format = format == TextureFormat::Unknown ? DXGI_FORMAT(texture.Format) : DXGI_FORMAT(format);
            desc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURECUBE;
            desc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
            desc.TextureCube.MipLevels = 1;
            desc.TextureCube.MostDetailedMip = 0;
        } else {
            desc.Buffer.FirstElement = 0;
            desc.Buffer.NumElements = resource->GetSize() / resource->GetStride();
            desc.Buffer.StructureByteStride = resource->GetStride();
        }

        device->GetDevice()->CreateShaderResourceView(resource->GetResource(), &desc, mDescriptor.CPU);
        break;
    }
    case ViewType::Storage: {
        mDescriptor = heaps[DescriptorHeapType::ShaderResource]->Allocate();

        D3D12_UNORDERED_ACCESS_VIEW_DESC desc = {};
        desc.ViewDimension = dimension == ViewDimension::Buffer ? D3D12_UAV_DIMENSION_BUFFER : D3D12_UAV_DIMENSION_TEXTURE2D;
        desc.Format = DXGI_FORMAT_UNKNOWN;
        if (dimension == ViewDimension::Texture) {
            TextureDesc texture = static_cast<Texture*>(resource.get())->GetDesc();
            desc.Format = format == TextureFormat::Unknown ? DXGI_FORMAT(texture.Format) : DXGI_FORMAT(format);
            desc.Texture2D.MipSlice = mip == VIEW_ALL_MIPS ? 0 : mip;
            desc.Texture2D.PlaneSlice = 0;
        } else if (dimension == ViewDimension::TextureCube) {
            TextureDesc texture = static_cast<Texture*>(resource.get())->GetDesc();
            desc.ViewDimension = D3D12_UAV_DIMENSION_TEXTURE2DARRAY;
            desc.Format = format == TextureFormat::Unknown ? DXGI_FORMAT(texture.Format) : DXGI_FORMAT(format);
            desc.Texture2DArray.ArraySize = 6;
            desc.Texture2DArray.FirstArraySlice = 0;
        } else {
            desc.Buffer.FirstElement = 0;
            desc.Buffer.NumElements = resource->GetSize();
            desc.Buffer.StructureByteStride = 1;
        }

        device->GetDevice()->CreateUnorderedAccessView(resource->GetResource(), nullptr, &desc, mDescriptor.CPU);
        break;
    }
    case ViewType::RenderTarget: {
        ASSERT(dimension != ViewDimension::Buffer, "Buffers cannot be render targets!");
        
        mDescriptor = heaps[DescriptorHeapType::RenderTarget]->Allocate();
        TextureDesc texture = static_cast<Texture*>(resource.get())->GetDesc();

        D3D12_RENDER_TARGET_VIEW_DESC desc = {};
        desc.Format = format == TextureFormat::Unknown ? DXGI_FORMAT(texture.Format) : DXGI_FORMAT(format);
        desc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;
        desc.Texture2D.PlaneSlice = depthSlice;

        device->GetDevice()->CreateRenderTargetView(resource->GetResource(), &desc, mDescriptor.CPU);
        break;
    }
    case ViewType::DepthTarget: {
        ASSERT(dimension != ViewDimension::Buffer, "Buffers cannot be depth targets!");
        
        mDescriptor = heaps[DescriptorHeapType::DepthTarget]->Allocate();
        TextureDesc texture = static_cast<Texture*>(resource.get())->GetDesc();

        D3D12_DEPTH_STENCIL_VIEW_DESC desc = {};
        if (dimension == ViewDimension::Texture) {
            desc.Format = format == TextureFormat::Unknown ? DXGI_FORMAT(texture.Format) : DXGI_FORMAT(format);
            desc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
        } else {
            desc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2DARRAY;
            desc.Format = DXGI_FORMAT(texture.Format);

            desc.Texture2DArray.ArraySize = 1;
            desc.Texture2DArray.FirstArraySlice = depthSlice;
            desc.Texture2DArray.MipSlice = 0;
        }
        
        device->GetDevice()->CreateDepthStencilView(resource->GetResource(), &desc, mDescriptor.CPU);
        break;
    }
    }
}

View::~View()
{
    mDescriptor.Parent->Free(mDescriptor);
}
