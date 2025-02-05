//
// > Notice: Amélie Heinrich @ 2025
// > Create Time: 2025-02-05 17:55:03
//

#include "Profiler.hpp"

#include <imgui.h>

Profiler::Data Profiler::sData = {};

// ProfilerEntry constructor
ProfilerEntry::ProfilerEntry(const String& name)
    : mName(name), mFrame(Profiler::sData.CurrentFrame)
{
    mTimer.Restart();
}

// Destructor automatically records elapsed time and pushes the entry
ProfilerEntry::~ProfilerEntry()
{
    mCPUTime = mTimer.GetElapsed();
    Profiler::PushEntry(*this);
}

// BeginFrame: Move to next frame
void Profiler::BeginFrame()
{
    sData.CurrentFrame++;
}

// PushEntry: Reuse existing entries if possible
void Profiler::PushEntry(const ProfilerEntry& entry)
{
    // Find an existing inactive entry (from an old frame)
    for (UInt64 i = 0; i < sData.EntryCount; ++i) {
        if (sData.Entries[i].GetFrame() != sData.CurrentFrame) {
            sData.Entries[i] = entry;
            return;
        }
    }

    // If no reusable entry, add a new one
    if (sData.EntryCount < MAX_PROFILER_ENTRIES) {
        sData.Entries[sData.EntryCount++] = entry;
        return;
    }

    // If the buffer is full, overwrite the oldest entry (ring buffer behavior)
    UInt64 index = sData.EntryCount % MAX_PROFILER_ENTRIES;
    sData.Entries[index] = entry;
}

// ImGui UI rendering
void Profiler::OnUI()
{
    ImGui::Begin("Profiler");
    if (ImGui::TreeNodeEx("CPU Profiler", ImGuiTreeNodeFlags_Framed)) {
        for (UInt64 i = 0; i < sData.EntryCount; ++i) {
            const ProfilerEntry& entry = sData.Entries[i];
            ImGui::Text("%s : %fms", entry.GetName().c_str(), entry.GetCPUTime());
        }
        ImGui::TreePop();
    }
    ImGui::End();
}
