//
// > Notice: Amélie Heinrich @ 2025
// > Create Time: 2025-12-03 11:32:04
//

#include <RHI/Device.hpp>
#include <RHI/View.hpp>
#include <RHI/Queue.hpp>
#include <Core/Window.hpp>

constexpr UInt32 FRAMES_IN_FLIGHT = 3;

class Surface
{
public:
    using Ref = Ref<Surface>;

    Surface(::Ref<Window> window, Device::Ref device, DescriptorHeaps heaps, Queue::Ref queue);
    ~Surface();

    void Present(bool vsync);

    UInt32 GetBackbufferIndex() { return mSwapchain->GetCurrentBackBufferIndex(); }
    Texture::Ref GetBackbuffer(UInt32 idx) { return mBackbuffers[idx]; }
    View::Ref GetBackbufferView(UInt32 idx) { return mBackbufferViews[idx]; }
private:
    IDXGISwapChain4* mSwapchain = nullptr;
    Array<Texture::Ref, FRAMES_IN_FLIGHT> mBackbuffers;
    Array<View::Ref, FRAMES_IN_FLIGHT> mBackbufferViews;
};
