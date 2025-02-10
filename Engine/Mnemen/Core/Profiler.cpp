//
// > Notice: Amélie Heinrich @ 2025
// > Create Time: 2025-02-05 17:55:03
//

#include "Profiler.hpp"

#include <RHI/Uploader.hpp>
#include <Core/Statistics.hpp>

#include <sstream>
#include <imgui.h>
#include <FontAwesome/FontAwesome.hpp>

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

void Profiler::SetResourceData(Util::UUID id, UInt32 width, UInt32 height, UInt32 depth, UInt32 levels)
{
    if (sData.Resources.empty())
        return;
    if (sData.Resources.count(id) == 0)
        return;
    sData.Resources[id].Width = width;
    sData.Resources[id].Height = height;
    sData.Resources[id].Depth = depth;
    sData.Resources[id].Levels = levels;
}

void Profiler::TagResource(Util::UUID id, ResourceTag tag)
{
    if (sData.Resources.empty())
        return;
    if (sData.Resources.count(id) == 0)
        return;
    sData.Resources[id].Tags.insert(tag);
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
    Statistics::Update();

    ImGui::Begin(ICON_FA_CLOCK_O " Profiler");
    if (ImGui::TreeNodeEx("Statistics", ImGuiTreeNodeFlags_Framed)) {
        ImGui::Text("Instance Count : %llu", Statistics::Get().InstanceCount);
        ImGui::Text("Triangle Count : %llu", Statistics::Get().TriangleCount);
        ImGui::Text("Meshlet Count : %llu", Statistics::Get().MeshletCount);
        ImGui::Text("Draw Call Count : %llu", Statistics::Get().DrawCallCount);
        ImGui::Text("Dispatch Count : %llu", Statistics::Get().DispatchCount);
        ImGui::Separator();
        // Resources
        // VRAM
        {
            UInt64 percentage = (Statistics::Get().UsedVRAM * 100) / Statistics::Get().MaxVRAM;
            float stupidVRAMPercetange = percentage / 100.0f;
            std::stringstream ss;
            ss << ICON_FA_VIDEO_CAMERA << " VRAM Usage (" << percentage << "%%): " << (((Statistics::Get().UsedVRAM / 1024.0F) / 1024.0f) / 1024.0f) << "gb/" << (((Statistics::Get().MaxVRAM / 1024.0f) / 1024.0f) / 1024.0f) << "gb";
            std::stringstream percents;
            percents << percentage << "%";
            ImGui::Text(ss.str().c_str());
            ImGui::ProgressBar(stupidVRAMPercetange, ImVec2(0, 0), percents.str().c_str());
        }
        ImGui::Separator();
        // RAM
        {
            UInt64 percentage = (Statistics::Get().UsedRAM * 100) / Statistics::Get().MaxRAM;
            float stupidRAMPercetange = percentage / 100.0f;
            std::stringstream ss;
            ss << ICON_FA_LAPTOP << " RAM Usage (" << percentage << "%%): " << (((Statistics::Get().UsedRAM / 1024.0F) / 1024.0f) / 1024.0f) << "gb/" << (((Statistics::Get().MaxRAM / 1024.0F) / 1024.0f) / 1024.0f) << "gb";
            std::stringstream percents;
            percents << percentage << "%";
            ImGui::Text(ss.str().c_str());
            ImGui::ProgressBar(stupidRAMPercetange, ImVec2(0, 0), percents.str().c_str());
        }
        ImGui::Separator();
        // Battery
        {
            std::stringstream ss;
            ss << ICON_FA_BATTERY_FULL << " Battery (" << Statistics::Get().Battery << "%%)";
            std::stringstream percentss;
            percentss << Statistics::Get().Battery << "%";
            ImGui::Text(ss.str().c_str());
            ImGui::ProgressBar(Statistics::Get().Battery / 100.0f, ImVec2(0, 0), percentss.str().c_str());
        }
        ImGui::TreePop();
    }
    if (ImGui::TreeNodeEx("CPU Profiler", ImGuiTreeNodeFlags_Framed)) {
        for (UInt64 i = 0; i < sData.EntryCount; ++i) {
            const ProfilerEntry& entry = sData.Entries[i];
            if (!entry.IsGPU()) {
                ImGui::Text("%s : %fms", entry.GetName().c_str(), entry.GetCPUTime());
            }
        }
        ImGui::TreePop();
    }
    if (ImGui::TreeNodeEx("GPU Resource Tree", ImGuiTreeNodeFlags_Framed)) {
        const char* tags[] = {
            ICON_FA_CUBE " Model Geometry",
            ICON_FA_PAINT_BRUSH " Material Texture",
            ICON_FA_SQUARE " Render Pass Shared",
            ICON_FA_WINDOW_CLOSE " Render Pass Not Shared",
            ICON_FA_TAG " GPU Readback"
        };

        for (int i = 0; i < (int)ResourceTag::MAX; i++) {
            ImGui::PushStyleColor(ImGuiCol_Header, (ImVec4)ImColor::HSV(i / 7.0f, 0.6f, 0.6f));
            ImGui::PushStyleColor(ImGuiCol_HeaderHovered, (ImVec4)ImColor::HSV(i / 7.0f, 0.7f, 0.7f));
            ImGui::PushStyleColor(ImGuiCol_HeaderActive, (ImVec4)ImColor::HSV(i / 7.0f, 0.8f, 0.8f));

            if (ImGui::TreeNodeEx(tags[i], ImGuiTreeNodeFlags_Framed)) {
                for (auto& item : sData.Resources) {
                    Util::UUID uuid = item.first;
                    ProfiledResource resource = item.second;
                    if (!resource.Tags.contains((ResourceTag)i))
                        continue;

                    ImGui::PushID((UInt64)uuid);
                    if (ImGui::TreeNode(resource.Name.c_str())) {
                        ImGui::Text("Size: %fmb", (float)(resource.Size / 1024.0f / 1024.0f));
                        ImGui::Text("Width: %u", resource.Width);
                        ImGui::Text("Height: %u", resource.Height);
                        ImGui::Text("Depth: %u", resource.Depth);
                        ImGui::Text("Mip Levels: %u", resource.Levels);
                        ImGui::TreePop();
                    }
                    ImGui::PopID();
                }
                ImGui::TreePop();
            }
            ImGui::PopStyleColor(3);
        }
        ImGui::TreePop();
    }
    ImGui::End();
}
