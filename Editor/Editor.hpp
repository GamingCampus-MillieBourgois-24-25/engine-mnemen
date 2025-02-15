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
    // Utility
    void UpdateShortcuts();
    void OpenScene(const String& path);
    void CloseScene();
    bool SaveScene();
    bool SaveSceneAs();
    void NewScene();
    void ReloadScene(const String& path);

    // UI
    void Viewport(const Frame& frame);
    void BeginDockSpace();
    void EndDockSpace();
    void HierarchyPanel();
    void SetColors();
    void AssetPanel();
    void EntityEditor();
    void AssetBrowser();
    void LogWindow();
    void FXVolumeEditor();
    void DrawVec3Control(const std::string& label, glm::vec3& values, float resetValue = 0.0f, float columnWidth = 100.0f);
    void DrawEntityNode(Entity entity);

    Camera mCamera;
    String mCurrentScenePath;

    // Entity stuff
    Entity mCameraEntity;
    Entity mSelectedEntity = {};
    PostProcessVolume* mSelectedVolume = nullptr;
    bool mMarkForDeletion = false;
    bool mMarkForClose = false;
    bool mMarkForStop = false;
    String mModelChange = "";
    UnorderedMap<entt::entity, String> mEntityNameBuffer;

    // Viewport shenanigans
    ImVec2 mViewportSize;
    ImVec2 mViewportBounds[2];
    bool mViewportFocused = false;
    bool mGizmoFocused = false;
    ImGuizmo::MODE mMode = ImGuizmo::MODE::WORLD;
    ImGuizmo::OPERATION mOperation = ImGuizmo::OPERATION::TRANSLATE;

    // Log Filtering
    ImGuiTextFilter mLogFilter;

    // Asset browing
    std::filesystem::path mBaseDirectory;
    std::filesystem::path mCurrentDirectory;
};
