//
// > Notice: Amélie Heinrich @ 2025
// > Create Time: 2025-12-03 06:23:27
//

#include <RHI/RHI.hpp>
#include <RHI/Uploader.hpp>

#include <imgui_impl_sdl3.h>
#include <imgui_impl_dx12.h>

#include <FontAwesome/FontAwesome.hpp>

#include <Core/Logger.hpp>
#include <Core/Profiler.hpp>
#include <Core/Statistics.hpp>

// NOTE(amelie): Courtesy of Robert Ryan A.K.A Implodee
DWORD AwaitFence(ID3D12Fence* fence, uint64_t val, uint64_t timeout)
{
    DWORD result = WAIT_FAILED;
    if (fence->GetCompletedValue() < val) {
        HANDLE eventHandle = CreateEvent(NULL, FALSE, FALSE, NULL);
        fence->SetEventOnCompletion(val, eventHandle);
        if (eventHandle != 0) {
            result = WaitForSingleObject(eventHandle, timeout);
            CloseHandle(eventHandle);
        }
    } else {
        result = WAIT_OBJECT_0;
    }
    return result;
}

DWORD AwaitQueue(ID3D12Device* device, ID3D12CommandQueue* queue, uint64_t timeout)
{
    ID3D12Fence1* fence = nullptr;
    device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&fence));
    if (!fence) {
        return WAIT_FAILED;
    }
    queue->Signal(fence, 1);
    auto result = AwaitFence(fence, 1, timeout);
    fence->Release();
    return result;
}

RHI::RHI(::Ref<Window> window)
    : mWindow(window)
{
    mDevice = MakeRef<Device>();

    mGraphicsQueue = MakeRef<Queue>(mDevice, QueueType::AllGraphics);
    
    mDescriptorHeaps[DescriptorHeapType::RenderTarget] = MakeRef<DescriptorHeap>(mDevice, DescriptorHeapType::RenderTarget, 2048);
    mDescriptorHeaps[DescriptorHeapType::DepthTarget] = MakeRef<DescriptorHeap>(mDevice, DescriptorHeapType::DepthTarget, 2048);
    mDescriptorHeaps[DescriptorHeapType::ShaderResource] = MakeRef<DescriptorHeap>(mDevice, DescriptorHeapType::ShaderResource, 1'000'000);
    mDescriptorHeaps[DescriptorHeapType::Sampler] = MakeRef<DescriptorHeap>(mDevice, DescriptorHeapType::Sampler, 2048);

    mSurface = MakeRef<Surface>(window, mDevice, mDescriptorHeaps, mGraphicsQueue);

    mFrameFence = MakeRef<Fence>(mDevice);
    mFrameIndex = 0;
    for (int i = 0; i < FRAMES_IN_FLIGHT; i++) {
        mFrameValues[i] = 0;
        mCommandBuffers[i] = MakeRef<CommandBuffer>(mDevice, mGraphicsQueue, mDescriptorHeaps);
    }

    Uploader::Init(this, mDevice, mDescriptorHeaps, mGraphicsQueue);

    mFontDescriptor = mDescriptorHeaps[DescriptorHeapType::ShaderResource]->Allocate();

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui::StyleColorsLight();

    ImGuiIO& IO = ImGui::GetIO();
    IO.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    IO.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
    IO.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

    ImFontConfig mergeConfig = {};
    mergeConfig.MergeMode = true;

    static const ImWchar rangesFixed[] = {
    	0x0020, 0x00FF, // Basic Latin + Latin Supplement
    	0x2026, 0x2026, // ellipsis
    	0
    };
    static const ImWchar rangesIcons[] = {
    	ICON_MIN_FA, ICON_MAX_FA,
    	0
    };

    IO.Fonts->AddFontFromFileTTF("Assets/Fonts/Roboto-Regular.ttf", 18, NULL, rangesFixed);
    IO.Fonts->AddFontFromFileTTF("Assets/Fonts/fontawesome-webfont.ttf", 14, &mergeConfig, rangesIcons);
    IO.Fonts->Build();

    mLargeFont = IO.Fonts->AddFontFromFileTTF("Assets/Fonts/fontawesome-webfont.ttf", 64, nullptr, rangesIcons);

    ImGui_ImplSDL3_InitForD3D(window->GetSDLHandle());
    ImGui_ImplDX12_InitInfo initInfo = {};
    initInfo.Device = mDevice->GetDevice();
    initInfo.CommandQueue = mGraphicsQueue->GetQueue();
    initInfo.NumFramesInFlight = FRAMES_IN_FLIGHT;
    initInfo.RTVFormat = DXGI_FORMAT_R8G8B8A8_UNORM;
    initInfo.DSVFormat = DXGI_FORMAT_UNKNOWN;
    initInfo.SrvDescriptorHeap = mDescriptorHeaps[DescriptorHeapType::ShaderResource]->GetHeap();
    initInfo.LegacySingleSrvCpuDescriptor = mFontDescriptor.CPU;
    initInfo.LegacySingleSrvGpuDescriptor = mFontDescriptor.GPU;
    ImGui_ImplDX12_Init(&initInfo);
    ImGui_ImplDX12_CreateDeviceObjects();
    
    LOG_INFO("Initialized D3D12 system");
}

RHI::~RHI()
{
    ImGui_ImplDX12_Shutdown();
    ImGui_ImplSDL3_Shutdown();
    ImGui::DestroyContext();
    mFontDescriptor.Parent->Free(mFontDescriptor);
}

void RHI::Wait()
{
    AwaitQueue(mDevice->GetDevice(), mGraphicsQueue->GetQueue(), 10'000'000);
}

void RHI::Submit(const Vector<CommandBuffer::Ref> buffers)
{
    mGraphicsQueue->Submit(buffers);
}

Frame RHI::Begin()
{
    Statistics::Reset();

    Frame frame = {};
    frame.FrameIndex = mSurface->GetBackbufferIndex();
    frame.Backbuffer = mSurface->GetBackbuffer(frame.FrameIndex);
    frame.BackbufferView = mSurface->GetBackbufferView(frame.FrameIndex);
    frame.CommandBuffer = mCommandBuffers[frame.FrameIndex];
    
    mFrameIndex = frame.FrameIndex;

    mWindow->PollSize(frame.Width, frame.Height);

    return frame;
}

void RHI::End()
{
    const UInt64 fenceValue = mFrameValues[mFrameIndex];
    mGraphicsQueue->Signal(mFrameFence, fenceValue);

    if (mFrameFence->GetCompletedValue() < mFrameValues[mFrameIndex]) {
        mFrameFence->Wait(mFrameValues[mFrameIndex]);
    }
    mFrameValues[mFrameIndex] = fenceValue + 1;
}

void RHI::Present(bool vsync)
{
    PROFILE_FUNCTION();
    mSurface->Present(vsync);
}

RootSignature::Ref RHI::CreateRootSignature()
{
    return MakeRef<RootSignature>(mDevice);
}

RootSignature::Ref RHI::CreateRootSignature(const Vector<RootType>& entries, UInt64 pushConstantSize)
{
    return MakeRef<RootSignature>(mDevice, entries, pushConstantSize);
}

GraphicsPipeline::Ref RHI::CreateGraphicsPipeline(GraphicsPipelineSpecs& specs)
{
    return MakeRef<GraphicsPipeline>(mDevice, specs);
}

MeshPipeline::Ref RHI::CreateMeshPipeline(GraphicsPipelineSpecs& specs)
{
    return MakeRef<MeshPipeline>(mDevice, specs);
}

ComputePipeline::Ref RHI::CreateComputePipeline(Shader shader, RootSignature::Ref signature)
{
    return MakeRef<ComputePipeline>(mDevice, shader, signature);
}

Buffer::Ref RHI::CreateBuffer(UInt64 size, UInt64 stride, BufferType type, const String& name)
{
    return MakeRef<Buffer>(mDevice, mDescriptorHeaps, size, stride, type, name);
}

Texture::Ref RHI::CreateTexture(TextureDesc desc)
{
    return MakeRef<Texture>(mDevice, desc);
}

View::Ref RHI::CreateView(::Ref<Resource> resource, ViewType type, ViewDimension dimension, TextureFormat format, UInt64 mip, UInt64 depthSlice)
{
    return MakeRef<View>(mDevice, mDescriptorHeaps, resource, type, dimension, format, mip, depthSlice);
}

Sampler::Ref RHI::CreateSampler(SamplerAddress address, SamplerFilter filter, bool mips, int anisotropyLevel, bool comparison)
{
    return MakeRef<Sampler>(mDevice, mDescriptorHeaps, address, filter, mips, anisotropyLevel, comparison);
}

BLAS::Ref RHI::CreateBLAS(Buffer::Ref vertex, Buffer::Ref index, UInt32 vtxCount, UInt32 idxCount, const String& name)
{
    return MakeRef<BLAS>(mDevice, mDescriptorHeaps, vertex, index, vtxCount, idxCount, name);
}

TLAS::Ref RHI::CreateTLAS(Buffer::Ref instanceBuffer, UInt32 numInstance, const String& name)
{
    return MakeRef<TLAS>(mDevice, mDescriptorHeaps, instanceBuffer, numInstance, name);
}
