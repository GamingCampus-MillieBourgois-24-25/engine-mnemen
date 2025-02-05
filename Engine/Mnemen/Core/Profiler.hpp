//
// > Notice: Amélie Heinrich @ 2025
// > Create Time: 2025-02-05 17:48:03
//

#pragma once

#include <Core/Common.hpp>
#include <Core/Timer.hpp>

constexpr size_t MAX_PROFILER_ENTRIES = 1024;

struct ProfilerEntry
{
public:
    ProfilerEntry() = default;
    ProfilerEntry(const String& name);
    ~ProfilerEntry();

    float GetCPUTime() const { return mCPUTime; }
    const String& GetName() const { return mName; }
    UInt64 GetFrame() const { return mFrame; }

private:
    friend class Profiler;

    Timer mTimer;
    String mName;
    float mCPUTime = 0.0f;
    UInt64 mFrame = 0;  // Frame this entry belongs to
};

class Profiler
{
public:
    static void BeginFrame();
    static void PushEntry(const ProfilerEntry& entry);
    static void OnUI();
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
