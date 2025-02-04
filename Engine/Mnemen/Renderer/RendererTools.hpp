//
// > Notice: Amélie Heinrich @ 2025
// > Create Time: 2025-02-04 10:37:18
//

#pragma once

#include <RHI/RHI.hpp>

typedef Array<Buffer::Ref, FRAMES_IN_FLIGHT> RingBuffer;

enum class RenderPassResourceType
{
    SharedTexture,
    SharedRingBuffer,
    SharedBuffer,
    SharedSampler
};

class RenderPassResource
{
public:
    RenderPassResourceType Type;
    Texture::Ref Texture;
    RingBuffer RBuffer;
    Buffer::Ref Buffer;
    Sampler::Ref Sampler;

    RenderPassResource() = default;
    ~RenderPassResource() = default;

    View::Ref AddView(ViewType type, ViewDimension dimension = ViewDimension::Texture, TextureFormat format = TextureFormat::Unknown);
    View::Ref GetView(ViewType type);
    int Descriptor(ViewType type = ViewType::None, int frameIndex = 0);
private:
    RHI::Ref ParentRHI;
    Vector<View::Ref> Views;

    friend class RendererTools;
};

class RendererTools
{
public:
    static void Init(RHI::Ref rhi);

    static Ref<RenderPassResource> CreateSharedTexture(const String& name, TextureDesc desc);
    static Ref<RenderPassResource> CreateSharedRingBuffer(const String& name, UInt64 size);
    static Ref<RenderPassResource> CreateSharedRWBuffer(const String& name, UInt64 size, UInt64 stride);
    static Ref<RenderPassResource> CreateSharedSampler(const String& name, SamplerFilter filter, SamplerAddress address, bool mips = false);
    static Ref<RenderPassResource> Get(const String& name);
private:
    struct RendererToolsData
    {
        RHI::Ref RHI;
        UnorderedMap<String, Ref<RenderPassResource>> Resources;
    };

    static RendererToolsData sData;
};
