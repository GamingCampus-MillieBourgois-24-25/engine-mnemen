//
// > Notice: Amélie Heinrich @ 2025
// > Create Time: 2025-02-11 19:05:52
//

#include "Editor.hpp"

#include <FontAwesome/FontAwesome.hpp>
#include <RHI/Uploader.hpp>
#include <Utility/Dialog.hpp>

#include <imgui.h>
#include <imgui_internal.h>

void Editor::UpdateShortcuts()
{
    if (Input::IsKeyDown(SDLK_LCTRL)) {
        if (Input::IsKeyPressed(SDLK_Q)) {
            SaveScene();
            mWindow->Close();
        }
        if (Input::IsKeyPressed(SDLK_S)) {
            SaveScene();
        }
        if (Input::IsKeyPressed(SDLK_N)) {
            SaveScene();
            NewScene();
        }
        if (Input::IsKeyPressed(SDLK_O)) {
            String path = Dialog::Open({ ".msf" });
            if (!path.empty()) {
                OpenScene(path);
            }
        }
    }
    if (Input::IsKeyPressed(SDLK_T)) {
        mMode = ImGuizmo::MODE::WORLD;
        mOperation = ImGuizmo::OPERATION::TRANSLATE;
    }
    if (Input::IsKeyPressed(SDLK_R)) {
        mMode = ImGuizmo::MODE::WORLD;
        mOperation = ImGuizmo::OPERATION::ROTATE;
    }
    if (Input::IsKeyPressed(SDLK_B)) {
        mMode = ImGuizmo::MODE::WORLD;
        mOperation = ImGuizmo::OPERATION::SCALE;
    }
    if (Input::IsKeyPressed(SDLK_ESCAPE)) {
        mSelectedEntity = {};
    }
}

void Editor::OpenScene(const String& path)
{
    CloseScene();

    mCurrentScenePath = path;
    mScene = SceneSerializer::DeserializeScene(mCurrentScenePath);
    
    mCameraEntity = mScene->AddEntity("Editor Camera");
    mCameraEntity.AddComponent<PrivateComponent>();
    
    auto& cam = mCameraEntity.AddComponent<CameraComponent>();
    cam.Primary = 2;
}

void Editor::CloseScene()
{
    SaveScene();

    mCurrentScenePath = {};
    mSelectedEntity = {};
    mScene.reset();
}

bool Editor::SaveScene()
{
    if (mCurrentScenePath.empty()) {
        return SaveSceneAs();
    }
    SceneSerializer::SerializeScene(mScene, mCurrentScenePath);
    return true;
}

bool Editor::SaveSceneAs()
{
    String savePath = Dialog::Save({ ".msf" });
    if (!savePath.empty()) {
        if (mCurrentScenePath.empty()) {
            mCurrentScenePath = savePath;
        }
        SceneSerializer::SerializeScene(mScene, savePath);
        return true;
    }
    return false;
}

void Editor::NewScene()
{
    if (mScene)
        CloseScene();
    mScene = MakeRef<Scene>();

    mCameraEntity = mScene->AddEntity("Editor Camera");
    mCameraEntity.AddComponent<PrivateComponent>();
    auto& cam = mCameraEntity.AddComponent<CameraComponent>();
    cam.Primary = 2;
}
