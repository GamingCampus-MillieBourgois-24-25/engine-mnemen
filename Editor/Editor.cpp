//
// > Notice: Amélie Heinrich @ 2025
// > Create Time: 2025-02-03 12:26:03
//

#include "Editor.hpp"

#include <FontAwesome/FontAwesome.hpp>
#include <RHI/Uploader.hpp>

#include <imgui.h>
#include <imgui_internal.h>

Editor::Editor(ApplicationSpecs specs)
    : Application(specs)
{
    mCurrentScenePath = specs.StartScene;
    if (mCurrentScenePath.empty())
        NewScene();
    mScenePlaying = false;

    SetColors();

    mBaseDirectory = "Assets";
    mCurrentDirectory = "Assets";
}

Editor::~Editor()
{

}

void Editor::OnUpdate(float dt)
{
    if (!mScene)
        return;

    int width, height;
    mWindow->PollSize(width, height);

    mCamera.UpdateMatrices(std::max(mViewportSize.x, 1.0f), std::max(mViewportSize.y, 1.0f));
    if (mViewportFocused && !mScenePlaying && !mGizmoFocused)
        mCamera.Input(dt);
    if (!mScenePlaying)
        UpdateShortcuts();

    auto& cam = mCameraEntity.GetComponent<CameraComponent>();
    cam.Primary = !mScenePlaying ? 2 : 0;
    cam.Projection = mCamera.Projection();
    cam.View = mCamera.View();
}

void Editor::PostPresent()
{
    // Change the model if needed
    if (!mModelChange.empty()) {
        if (mSelectedEntity) {
            MeshComponent& mesh = mSelectedEntity.GetComponent<MeshComponent>();
            mesh.Init(mModelChange);
        }
        mModelChange = "";
    }
    // Delete the entity if needed
    if (mMarkForDeletion) {
        mScene->RemoveEntity(mSelectedEntity);
        mSelectedEntity = nullptr;
        mMarkForDeletion = false;
    }
    // Purge unused assets every frame
    AssetManager::Purge();

    // New scene if needed
    if (mMarkForClose) {
        NewScene();
        mMarkForClose = false;
    }
    // Upload after new scene
    Uploader::Flush();
}

void Editor::OnPhysicsTick()
{

}

void Editor::OnImGui(const Frame& frame)
{
    if (!mScene)
        return;

    PROFILE_FUNCTION();

    BeginDockSpace();
    Profiler::OnUI();
    mRenderer->UI(frame);
    Viewport(frame);
    LogWindow();
    HierarchyPanel();
    AssetPanel();
    EntityEditor();
    EndDockSpace();
}
