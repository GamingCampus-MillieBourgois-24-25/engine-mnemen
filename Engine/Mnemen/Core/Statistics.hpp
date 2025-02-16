//
// > Notice: Amélie Heinrich @ 2025
// > Create Time: 2025-02-09 18:15:37
//

#pragma once

#include <Core/Common.hpp>

/// @struct Statistics
/// @brief A structure to hold performance and resource usage statistics.
///
/// This structure stores various counts and usage statistics such as the number of instances,
/// triangles, meshlets, dispatch calls, draw calls, and memory usage (VRAM and RAM). It also
/// holds battery information, and provides utility functions for resetting and updating the statistics.
struct Statistics
{
    /// @brief The total number of instances.
    UInt64 InstanceCount = 0;

    /// @brief The total number of triangles.
    UInt64 TriangleCount = 0;

    /// @brief The total number of meshlets.
    UInt64 MeshletCount = 0;

    /// @brief The total number of dispatch calls.
    UInt64 DispatchCount = 0;

    /// @brief The total number of draw calls.
    UInt64 DrawCallCount = 0;

    /// @brief The amount of used VRAM in bytes.
    UInt64 UsedVRAM = 0;

    /// @brief The maximum VRAM available in bytes.
    UInt64 MaxVRAM = 0;

    /// @brief The amount of used RAM in bytes.
    UInt64 UsedRAM = 0;

    /// @brief The maximum RAM available in bytes.
    UInt64 MaxRAM = 0;

    /// @brief The battery percentage.
    int Battery = 0;

    /// @brief Resets the statistics to initial values.
    ///
    /// This static function resets the various counts (e.g., draw calls, instances, triangles) to zero.
    static void Reset()
    {
        Statistics& stats = Get();
        stats.DrawCallCount = 0;
        stats.InstanceCount = 0;
        stats.TriangleCount = 0;
        stats.DispatchCount = 0;
        stats.MeshletCount = 0;
    }

    /// @brief Retrieves the singleton instance of the Statistics structure.
    ///
    /// This static function returns a reference to a static instance of Statistics, ensuring
    /// that only one instance exists during the application's runtime.
    ///
    /// @return A reference to the singleton Statistics instance.
    static Statistics& Get()
    {
        static Statistics stats;
        return stats;
    }

    /// @brief Updates the statistics.
    ///
    /// This static function is responsible for updating the statistics (e.g., VRAM usage, RAM usage).
    static void Update();
};
