//
// > Notice: Amélie Heinrich @ 2025
// > Create Time: 2025-02-05 19:00:51
//

#pragma once

#include "RHI.hpp"
#include "CommandBuffer.hpp"

class GPUTimer
{
public:
    static void Init(RHI::Ref rhi);
    static void Exit();

    static void Start(CommandBuffer::Ref cmdBuffer, UInt32 timerIndex);
    static void Stop(CommandBuffer::Ref cmdBuffer, UInt32 timerIndex);
    static void Resolve(CommandBuffer::Ref cmdBuffer);
    static void Readback();
    static double GetTime(UInt32 timerIndex);
private:
    struct Data {
        RHI::Ref RHI;

        ID3D12QueryHeap* QueryHeap;
        ID3D12Resource* ReadbackBuffer;
        Vector<UInt64> Timestamps;
        UInt32 MaxTimers = 0;
        UInt64 Frequency = 0;
    };
    static Data sData;
};
