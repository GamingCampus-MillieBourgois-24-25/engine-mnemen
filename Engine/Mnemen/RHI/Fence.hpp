//
// > Notice: Amélie Heinrich @ 2025
// > Create Time: 2025-12-03 10:01:44
//

#pragma once

#include <Core/Common.hpp>
#include <RHI/Device.hpp>

class Queue;

/// @brief Synchronization primitive for GPU execution.
class Fence
{
public:
    /// @brief Type alias for referencing
    using Ref = Ref<Fence>;

    /// @brief Creates a new fence for GPU synchronization.
    /// @param device The Direct3D 12 device.
    Fence(Device::Ref device);

    /// @brief Destroys the fence and releases resources.
    ~Fence();

    /// @brief Blocks the CPU until the GPU reaches the specified fence value.
    /// @param value The fence value to wait for.
    void Wait(UInt64 value);

    /// @brief Signals the fence with a new value from a given queue.
    /// @param queue The queue to signal from.
    /// @return The new fence value.
    UInt64 Signal(::Ref<Queue> queue);

    /// @brief Retrieves the most recently completed fence value.
    /// @return The completed fence value.
    UInt64 GetCompletedValue() { return mFence->GetCompletedValue(); }

    /// @brief Retrieves the underlying Direct3D 12 fence object.
    /// @return Pointer to the ID3D12Fence.
    ID3D12Fence* GetFence() { return mFence; }

    /// @brief Gets the current fence value.
    /// @return The current fence value.
    UInt64 GetValue() { return mValue; }

private:
    UInt64 mValue = 0; ///< The current fence value.
    ID3D12Fence* mFence = nullptr; ///< The Direct3D 12 fence object.
};
