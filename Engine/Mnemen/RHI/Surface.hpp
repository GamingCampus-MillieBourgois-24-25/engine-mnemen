//
// > Notice: Amélie Heinrich @ 2025
// > Create Time: 2025-12-03 11:32:04
//

#include <RHI/Device.hpp>
#include <RHI/View.hpp>
#include <RHI/Queue.hpp>
#include <Core/Window.hpp>

/// @brief Number of frames in flight (double buffering, triple buffering)
constexpr UInt32 FRAMES_IN_FLIGHT = 3;

/// @brief Represents a surface (swap chain) that can be presented to the screen.
/// @details The Surface class handles the swap chain and backbuffers, providing the ability
///          to present images to the display and access the current backbuffer for rendering.
///          It supports multiple frames in flight for efficient rendering.
class Surface
{
public:
    /// @brief Type alias for referencing
    using Ref = Ref<Surface>;

    /// @brief Constructs a surface (swap chain) for the specified window, device, descriptor heaps,
    ///        and command queue.
    /// @param window The window associated with the surface, used for creating the swap chain.
    /// @param device The device used for creating the swap chain and managing resources.
    /// @param heaps Descriptor heaps used by the swap chain.
    /// @param queue The queue used to present the swap chain.
    Surface(::Ref<Window> window, Device::Ref device, DescriptorHeaps heaps, Queue::Ref queue);

    /// @brief Destructor that cleans up the resources associated with the surface.
    ~Surface();

    /// @brief Presents the current backbuffer to the screen.
    /// @param vsync If true, enables vertical synchronization to prevent tearing.
    void Present(bool vsync);

    /// @brief Retrieves the index of the current backbuffer.
    /// @return The index of the current backbuffer.
    UInt32 GetBackbufferIndex() { return mSwapchain->GetCurrentBackBufferIndex(); }

    /// @brief Retrieves a reference to a backbuffer at the specified index.
    /// @param idx The index of the backbuffer to retrieve.
    /// @return A reference to the specified backbuffer.
    Texture::Ref GetBackbuffer(UInt32 idx) { return mBackbuffers[idx]; }

    /// @brief Retrieves a reference to the view of a backbuffer at the specified index.
    /// @param idx The index of the backbuffer view to retrieve.
    /// @return A reference to the specified backbuffer view.
    View::Ref GetBackbufferView(UInt32 idx) { return mBackbufferViews[idx]; }

private:
    IDXGISwapChain4* mSwapchain = nullptr; ///< The swap chain interface for presenting images to the screen.
    Array<Texture::Ref, FRAMES_IN_FLIGHT> mBackbuffers; ///< Array of backbuffers for double/triple buffering.
    Array<View::Ref, FRAMES_IN_FLIGHT> mBackbufferViews; ///< Array of views for the backbuffers.
};

