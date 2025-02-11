//
// > Notice: Amélie Heinrich @ 2025
// > Create Time: 2025-02-03 12:23:59
//

#pragma once

#include <Mnemen.hpp>
#include <imgui.h>
#include <filesystem>
#include <ImGuizmo/ImGuizmo.h>

#include "EditorCamera.hpp"

class Editor : public Application
{
public:
    Editor(ApplicationSpecs specs);
    ~Editor();

    virtual void OnUpdate(float dt) override;
    virtual void OnPhysicsTick() override;
    virtual void OnImGui(const Frame& frame) override;
    virtual void PostPresent() override;
private:
    void Viewport(const Frame& frame);
    void BeginDockSpace();
    void HierarchyPanel();
    void SetColors();
    void AssetPanel();
    void EntityEditor();
    void AssetBrowser();
    void LogWindow();
    void UpdateShortcuts();
    void DrawVec3Control(const std::string& label, glm::vec3& values, float resetValue = 0.0f, float columnWidth = 100.0f);

    Camera mCamera;
    String mCurrentScenePath;

    // Entity stuff
    Entity* mCameraEntity;
    Entity* mSelectedEntity = nullptr;
    bool mMarkForDeletion = false;
    String mModelChange = "";
    char mInputField[512];

    // Viewport shenanigans
    ImVec2 mViewportSize;
    ImVec2 mViewportBounds[2];
    bool mViewportFocused = false;
    bool mGizmoFocused = false;
    ImGuizmo::OPERATION mOperation = ImGuizmo::OPERATION::TRANSLATE;

    // Log Filtering
    ImGuiTextFilter mLogFilter;

    // Asset browing
    std::filesystem::path mBaseDirectory;
    std::filesystem::path mCurrentDirectory;
};
