//
// > Notice: Amélie Heinrich @ 2025
// > Create Time: 2025-02-05 17:48:03
//

#pragma once

#include <Core/Common.hpp>
#include <Core/Timer.hpp>

#include <RHI/CommandBuffer.hpp>
#include <RHI/GPUTimer.hpp>

constexpr size_t MAX_PROFILER_ENTRIES = 1024;

struct ProfilerEntry
{
public:
    ProfilerEntry() = default;
    ProfilerEntry(const String& name);
    ProfilerEntry(const String& name, CommandBuffer::Ref commandBuffer);
    ~ProfilerEntry();

    float GetCPUTime() const { return mCPUTime; }
    float GetGPUTime() const { return mGPUTime; }
    const String& GetName() const { return mName; }
    UInt64 GetFrame() const { return mFrame; }

    const bool IsGPU() const { return mGPU; }
private:
    friend class Profiler;

    Timer mTimer;
    String mName;
    float mCPUTime = 0.0f;
    float mGPUTime = 0.0f;
    bool mGPU = false;
    UInt32 mTimerIndex = 0;
    UInt64 mFrame = 0;
    CommandBuffer::Ref mCommandBuffer;
};

class Profiler
{
public:
    static void Init(RHI::Ref rhi);
    static void Exit();

    static void BeginFrame();
    static void PushEntry(const ProfilerEntry& entry);
    static void OnUI();

    static UInt32 StartGPUTimer(CommandBuffer::Ref cmdList);
    static void StopGPUTimer(CommandBuffer::Ref cmdList, UInt32 timerIndex);
    static void ResolveGPUQueries(CommandBuffer::Ref cmdList);
    static void ReadbackGPUResults();
private:
    friend class ProfilerEntry;

    struct Data {
        ProfilerEntry Entries[MAX_PROFILER_ENTRIES];
        UInt64 EntryCount = 0;
        UInt64 CurrentFrame = 0;  // Track current frame number
    };
    static Data sData;
};

#define PROFILE_FUNCTION() ProfilerEntry entry(__FUNCTION__)
#define PROFILE_SCOPE(name) ProfilerEntry entry(name)
#define PROFILE_SCOPE_GPU(name, list) ProfilerEntry entry(name, list)
