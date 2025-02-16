//
// > Notice: Amélie Heinrich @ 2025
// > Create Time: 2025-02-05 19:00:51
//

#pragma once

#include "RHI.hpp"
#include "CommandBuffer.hpp"

/// @class GPUTimer
/// @brief A GPU timer utility class for measuring execution time on the GPU.
class GPUTimer
{
public:
    /// @brief Initializes the GPU timer with the provided RHI reference.
    /// @param rhi Reference to the RHI instance.
    static void Init(RHI::Ref rhi);

    /// @brief Cleans up resources associated with the GPU timer.
    static void Exit();

    /// @brief Starts a GPU timer at the given index.
    /// @param cmdBuffer Reference to the command buffer.
    /// @param timerIndex Index of the timer to start.
    static void Start(CommandBuffer::Ref cmdBuffer, UInt32 timerIndex);

    /// @brief Stops a GPU timer at the given index.
    /// @param cmdBuffer Reference to the command buffer.
    /// @param timerIndex Index of the timer to stop.
    static void Stop(CommandBuffer::Ref cmdBuffer, UInt32 timerIndex);

    /// @brief Resolves all active timers in the provided command buffer.
    /// @param cmdBuffer Reference to the command buffer.
    static void Resolve(CommandBuffer::Ref cmdBuffer);

    /// @brief Reads back the resolved timing data from the GPU.
    static void Readback();

    /// @brief Retrieves the elapsed time for a given timer index.
    /// @param timerIndex Index of the timer.
    /// @return Elapsed time in milliseconds.
    static double GetTime(UInt32 timerIndex);

private:
    /// @struct Data
    /// @brief Internal data structure for managing GPU timer resources.
    struct Data {
        RHI::Ref RHI; ///< Reference to the RHI instance.

        ID3D12QueryHeap* QueryHeap; ///< Pointer to the Direct3D12 query heap.
        ID3D12Resource* ReadbackBuffer; ///< Pointer to the readback buffer resource.
        Vector<UInt64> Timestamps; ///< Vector storing timestamp values.
        UInt32 MaxTimers = 0; ///< Maximum number of timers supported.
        UInt64 Frequency = 0; ///< GPU timestamp frequency.
    };

    static Data sData; ///< Static instance of the internal data structure.
};
