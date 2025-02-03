//
// > Notice: Amélie Heinrich @ 2024
// > Create Time: 2024-12-03 06:22:48
//

#pragma once

#include <Core/Window.hpp>

#include <RHI/Device.hpp>
#include <RHI/DescriptorHeap.hpp>
#include <RHI/Queue.hpp>
#include <RHI/Fence.hpp>
#include <RHI/Surface.hpp>
#include <RHI/CommandBuffer.hpp>
#include <RHI/GraphicsPipeline.hpp>
#include <RHI/ComputePipeline.hpp>
#include <RHI/Buffer.hpp>
#include <RHI/Texture.hpp>
#include <RHI/Sampler.hpp>
#include <RHI/BLAS.hpp>
#include <RHI/TLAS.hpp>

struct Frame
{
    CommandBuffer::Ref CommandBuffer;
    Texture::Ref Backbuffer;
    View::Ref BackbufferView;
    UInt32 FrameIndex;

    Int32 Width;
    Int32 Height;
};

class RHI
{
public:
    using Ref = Ref<RHI>;

    RHI(::Ref<Window> window);
    ~RHI();

    void Wait();
    void Submit(const Vector<CommandBuffer::Ref> buffers);

    Frame Begin();
    void End();
    void Present(bool vsync);

    RootSignature::Ref CreateRootSignature();
    RootSignature::Ref CreateRootSignature(const Vector<RootType>& entries, UInt64 pushConstantSize = 0);
    
    GraphicsPipeline::Ref CreateGraphicsPipeline(GraphicsPipelineSpecs& specs);

    ComputePipeline::Ref CreateComputePipeline(Shader shader, RootSignature::Ref signature);
    
    Buffer::Ref CreateBuffer(UInt64 size, UInt64 stride, BufferType type, const String& name = "Buffer");
    
    Texture::Ref CreateTexture(TextureDesc desc);
    
    View::Ref CreateView(::Ref<Resource> resource, ViewType type, ViewDimension dimension = ViewDimension::Texture, TextureFormat format = TextureFormat::Unknown, UInt64 mip = VIEW_ALL_MIPS, UInt64 depthSlice = 0);
    
    Sampler::Ref CreateSampler(SamplerAddress address, SamplerFilter filter, bool mips = false, int anisotropyLevel = 4, bool comparison = false);

    BLAS::Ref CreateBLAS(Buffer::Ref vertex, Buffer::Ref index, UInt32 vtxCount, UInt32 idxCount, const String& name = "BLAS");
    TLAS::Ref CreateTLAS(Buffer::Ref instanceBuffer, UInt32 numInstance, const String& name = "TLAS");
private:
    ::Ref<Window> mWindow = nullptr;
    Device::Ref mDevice = nullptr;
    Queue::Ref mGraphicsQueue = nullptr;
    DescriptorHeaps mDescriptorHeaps;
    Surface::Ref mSurface = nullptr;

    Fence::Ref mFrameFence = nullptr;
    Array<UInt64, FRAMES_IN_FLIGHT> mFrameValues;
    Array<CommandBuffer::Ref, FRAMES_IN_FLIGHT> mCommandBuffers;
    UInt32 mFrameIndex = 0;

    DescriptorHeap::Descriptor mFontDescriptor;
};
