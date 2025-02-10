//
// > Notice: Amélie Heinrich @ 2025
// > Create Time: 2025-02-09 18:15:37
//

#pragma once

#include <Core/Common.hpp>

struct Statistics
{
    UInt64 InstanceCount = 0;
    UInt64 TriangleCount = 0;
    UInt64 MeshletCount = 0;
    UInt64 DispatchCount = 0;
    UInt64 DrawCallCount = 0;

    UInt64 UsedVRAM = 0;
    UInt64 MaxVRAM = 0;

    UInt64 UsedRAM = 0;
    UInt64 MaxRAM = 0;

    int Battery = 0;

    static void Reset()
    {
        Statistics& stats = Get();
        stats.DrawCallCount = 0;
        stats.InstanceCount = 0;
        stats.TriangleCount = 0;
        stats.DispatchCount = 0;
        stats.MeshletCount = 0;
    }

    static Statistics& Get()
    {
        static Statistics stats;
        return stats;
    }

    static void Update();
};
