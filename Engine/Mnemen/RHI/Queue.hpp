//
// > Notice: Amélie Heinrich @ 2025
// > Create Time: 2025-12-03 10:08:14
//

#pragma once

#include <RHI/Device.hpp>

class Fence;
class CommandBuffer;

/// @brief Represents the types of command queues in Direct3D 12.
enum class QueueType
{
    /// @brief Graphics command queue for general rendering tasks.
    AllGraphics = D3D12_COMMAND_LIST_TYPE_DIRECT,
    /// @brief Compute command queue for dispatching compute shaders.
    Compute = D3D12_COMMAND_LIST_TYPE_COMPUTE,
    /// @brief Copy command queue for handling copy operations.
    Copy = D3D12_COMMAND_LIST_TYPE_COPY,
    /// @brief Video decode command queue for handling video decoding operations.
    VideoDecode = D3D12_COMMAND_LIST_TYPE_VIDEO_DECODE,
    /// @brief Video encode command queue for handling video encoding operations.
    VideoEncode = D3D12_COMMAND_LIST_TYPE_VIDEO_ENCODE
};

/// @brief Represents a command queue in Direct3D 12, managing command execution on different types of queues (graphics, compute, copy, etc.)
class Queue
{
public:
    /// @brief Type alias for referencing
    using Ref = Ref<Queue>;

    /// @brief Creates a command queue with the specified device and queue type.
    /// @param device The Direct3D 12 device used for queue creation.
    /// @param type The type of queue (graphics, compute, copy, etc.).
    Queue(Device::Ref device, QueueType type);

    /// @brief Destroys the command queue.
    ~Queue();

    /// @brief Waits for a specific fence value to be completed before proceeding.
    /// @param fence The fence to wait on.
    /// @param value The value at which the fence is signaled.
    void Wait(::Ref<Fence> fence, UInt64 value);

    /// @brief Signals a fence with a specific value to notify its completion.
    /// @param fence The fence to signal.
    /// @param value The value to signal the fence at.
    void Signal(::Ref<Fence> fence, UInt64 value);

    /// @brief Submits a vector of command buffers to the command queue for execution.
    /// @param buffers A vector of command buffers to be executed.
    void Submit(const Vector<::Ref<CommandBuffer>>& buffers);

    /// @brief Retrieves the Direct3D 12 command queue object.
    /// @return The underlying command queue object.
    ID3D12CommandQueue* GetQueue() { return mQueue; }

    /// @brief Retrieves the type of this command queue.
    /// @return The queue type (graphics, compute, etc.).
    QueueType GetType() { return mType; }

private:
    ID3D12CommandQueue* mQueue = nullptr; ///< Direct3D 12 command queue object.
    QueueType mType; ///< The type of queue (graphics, compute, copy, etc.).
};
