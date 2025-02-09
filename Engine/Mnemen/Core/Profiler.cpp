//
// > Notice: Amélie Heinrich @ 2025
// > Create Time: 2025-02-05 17:55:03
//

#include "Profiler.hpp"

#include <imgui.h>
#include <RHI/Uploader.hpp>

Profiler::Data Profiler::sData = {};

// ProfilerEntry constructor
ProfilerEntry::ProfilerEntry(const String& name)
    : mName(name), mFrame(Profiler::sData.CurrentFrame)
{
    mTimer.Restart();
}

ProfilerEntry::ProfilerEntry(const String& name, CommandBuffer::Ref commandBuffer)
    : mName(name), mFrame(Profiler::sData.CurrentFrame)
{
    mGPU = true;
    mTimerIndex = Profiler::StartGPUTimer(commandBuffer);
    mCommandBuffer = commandBuffer;
}

// Destructor automatically records elapsed time and pushes the entry
ProfilerEntry::~ProfilerEntry()
{
    mCPUTime = mTimer.GetElapsed();
    if (mGPU) {
        Profiler::StopGPUTimer(mCommandBuffer, mTimerIndex);
    }
    Profiler::PushEntry(*this);
}

void Profiler::Init(RHI::Ref rhi)
{
    GPUTimer::Init(rhi);
}

void Profiler::Exit()
{
    sData.EntryCount = 0;
    sData.Resources.clear();
    GPUTimer::Exit();
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

UInt32 Profiler::StartGPUTimer(CommandBuffer::Ref cmdList)
{
    static UInt32 timerIndex = 0;
    UInt32 index = timerIndex++ % 256;
    GPUTimer::Start(cmdList, index);
    return index;
}

void Profiler::StopGPUTimer(CommandBuffer::Ref cmdList, UInt32 timerIndex)
{
    GPUTimer::Stop(cmdList, timerIndex);
}

void Profiler::ResolveGPUQueries(CommandBuffer::Ref cmdList)
{
    GPUTimer::Resolve(cmdList);
}

void Profiler::ReadbackGPUResults()
{
    GPUTimer::Readback();
}

Util::UUID Profiler::PushResource(UInt64 size, String Name)
{
    Util::UUID uuid = Util::NewUUID();
    sData.Resources[uuid] = {
        size, Name
    };
    return uuid;
}

void Profiler::PopResource(Util::UUID id)
{
    if (sData.Resources.empty())
        return;
    if (sData.Resources.count(id) == 0)
        return;
    sData.Resources.erase(id);
}

// ImGui UI rendering
void Profiler::OnUI()
{
    ImGui::Begin("Profiler");
    if (ImGui::TreeNodeEx("CPU Profiler", ImGuiTreeNodeFlags_Framed)) {
        for (UInt64 i = 0; i < sData.EntryCount; ++i) {
            const ProfilerEntry& entry = sData.Entries[i];
            if (!entry.IsGPU()) {
                ImGui::Text("%s : %fms", entry.GetName().c_str(), entry.GetCPUTime());
            }
        }
        ImGui::TreePop();
    }
    if (ImGui::TreeNodeEx("GPU Profiler", ImGuiTreeNodeFlags_Framed)) {
        for (UInt64 i = 0; i < sData.EntryCount; ++i) {
            const ProfilerEntry& entry = sData.Entries[i];
            if (entry.IsGPU()) {
                ImGui::Text("%s : %fms", entry.GetName().c_str(), entry.GetGPUTime());
            }
        }
        ImGui::TreePop();
    }
    if (ImGui::TreeNodeEx("Resource Tree", ImGuiTreeNodeFlags_Framed)) {
        for (auto& item : sData.Resources) {
            Util::UUID uuid = item.first;
            ProfiledResource resource = item.second;
            ImGui::PushID((UInt64)uuid);
            if (ImGui::TreeNode(resource.Name.c_str())) {
                ImGui::Text("Size: %fmb", (float)(resource.Size / 1024.0f / 1024.0f));
                ImGui::TreePop();
            }
            ImGui::PopID();
        }
        ImGui::TreePop();
    }
    ImGui::End();
}
