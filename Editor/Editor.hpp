//
// > Notice: Amélie Heinrich @ 2025
// > Create Time: 2025-02-03 12:23:59
//

#pragma once

#include <Mnemen.hpp>
#include <imgui.h>
#include <filesystem>

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

    Entity* mCameraEntity;
    Entity* mSelectedEntity = nullptr;
    bool mMarkForDeletion = false;
    String mModelChange = "";
    char mInputField[512];

    ImVec2 mViewportSize;
    bool mViewportFocused = false;
    ImGuiTextFilter mLogFilter;

    std::filesystem::path mBaseDirectory;
    std::filesystem::path mCurrentDirectory;
};
