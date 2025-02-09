//
// > Notice: Amélie Heinrich @ 2025
// > Create Time: 2025-02-05 17:48:03
//

#pragma once

#include <Core/Common.hpp>
#include <Core/Timer.hpp>
#include <Utility/UUID.hpp>

#include <RHI/CommandBuffer.hpp>
#include <RHI/GPUTimer.hpp>

constexpr size_t MAX_PROFILER_ENTRIES = 1024;

/// @struct ProfilerEntry
/// @brief Represents a single profiling entry for CPU and GPU execution times.
struct ProfilerEntry
{
public:
    /// @brief Default constructor.
    ProfilerEntry() = default;

    /// @brief Constructs a CPU profiler entry.
    /// @param name The name of the profiler entry.
    ProfilerEntry(const String& name);

    /// @brief Constructs a GPU profiler entry.
    /// @param name The name of the profiler entry.
    /// @param commandBuffer The command buffer associated with the GPU query.
    ProfilerEntry(const String& name, CommandBuffer::Ref commandBuffer);

    /// @brief Destructor.
    ~ProfilerEntry();

    /// @brief Gets the CPU execution time.
    /// @return CPU execution time in milliseconds.
    float GetCPUTime() const { return mCPUTime; }

    /// @brief Gets the GPU execution time.
    /// @return GPU execution time in milliseconds.
    float GetGPUTime() const { return mGPUTime; }

    /// @brief Gets the name of the profiling entry.
    /// @return The name of the entry.
    const String& GetName() const { return mName; }

    /// @brief Gets the frame index for this entry.
    /// @return The frame index.
    UInt64 GetFrame() const { return mFrame; }

    /// @brief Checks if this is a GPU profiling entry.
    /// @return True if it's a GPU entry, false otherwise.
    const bool IsGPU() const { return mGPU; }

private:
    friend class Profiler;

    Timer mTimer;                 ///< Timer object to measure CPU time.
    String mName;                 ///< Name of the profiler entry.
    float mCPUTime = 0.0f;        ///< CPU execution time in milliseconds.
    float mGPUTime = 0.0f;        ///< GPU execution time in milliseconds.
    bool mGPU = false;            ///< Flag to determine if it's a GPU entry.
    UInt32 mTimerIndex = 0;       ///< Index for the GPU timer.
    UInt64 mFrame = 0;            ///< Frame index.
    CommandBuffer::Ref mCommandBuffer; ///< Associated command buffer for GPU profiling.
};

/// @brief A resource displayed by the profiler
struct ProfiledResource
{
    /// @brief The size of the resource
    UInt64 Size;
    /// @brief The name of the resource
    String Name;
};

/// @class Profiler
/// @brief Manages CPU and GPU profiling entries, including GPU timing queries.
class Profiler
{
public:
    /// @brief Initializes the profiler system.
    /// @param rhi The rendering hardware interface (RHI) reference.
    static void Init(RHI::Ref rhi);

    /// @brief Shuts down the profiler system.
    static void Exit();

    /// @brief Marks the beginning of a new frame for profiling.
    static void BeginFrame();

    /// @brief Pushes a new profiling entry into the profiler.
    /// @param entry The profiling entry to be recorded.
    static void PushEntry(const ProfilerEntry& entry);

    /// @brief Displays profiling data in a UI panel.
    static void OnUI();

    /// @brief Starts a GPU timing query.
    /// @param cmdList The command buffer for GPU execution.
    /// @return The index of the GPU timer.
    static UInt32 StartGPUTimer(CommandBuffer::Ref cmdList);

    /// @brief Stops a GPU timing query.
    /// @param cmdList The command buffer for GPU execution.
    /// @param timerIndex The index of the GPU timer to stop.
    static void StopGPUTimer(CommandBuffer::Ref cmdList, UInt32 timerIndex);

    /// @brief Resolves GPU queries for timing information.
    /// @param cmdList The command buffer to execute query resolution.
    static void ResolveGPUQueries(CommandBuffer::Ref cmdList);

    /// @brief Reads back GPU timing results for processing.
    static void ReadbackGPUResults();

    /// @brief Pushes a resource in the render list
    static Util::UUID PushResource(UInt64 size, String Name);

    /// @brief Pops a resource in the render list
    static void PopResource(Util::UUID id);
private:
    friend class ProfilerEntry;

    /// @brief Internal profiler data structure.
    struct Data {
        ProfilerEntry Entries[MAX_PROFILER_ENTRIES]; ///< Array of profiling entries.
        UInt64 EntryCount = 0; ///< Number of active profiling entries.
        UInt64 CurrentFrame = 0; ///< Current frame index.
        UnorderedMap<Util::UUID, ProfiledResource> Resources; ///< List of profiled resources
    };

    static Data sData; ///< Static instance of profiler data.
};

/// @def PROFILE_FUNCTION()
/// @brief Creates a profiler entry for the current function.
/// @note Uses the function name as the profiling entry name.
#define PROFILE_FUNCTION() ProfilerEntry entry(__FUNCTION__)

/// @def PROFILE_SCOPE(name)
/// @brief Creates a profiler entry for a custom named scope.
/// @param name The name of the profiling entry.
#define PROFILE_SCOPE(name) ProfilerEntry entry(name)

/// @def PROFILE_SCOPE_GPU(name, list)
/// @brief Creates a profiler entry for GPU profiling within a named scope.
/// @param name The name of the profiling entry.
/// @param list The command buffer associated with the GPU execution.
#define PROFILE_SCOPE_GPU(name, list) ProfilerEntry entry(name, list)
