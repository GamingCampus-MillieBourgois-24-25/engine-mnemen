//
// > Notice: Amélie Heinrich @ 2025
// > Create Time: 2025-02-11 19:05:52
//

#include "Editor.hpp"

#include <FontAwesome/FontAwesome.hpp>
#include <RHI/Uploader.hpp>

#include <imgui.h>
#include <imgui_internal.h>

void Editor::UpdateShortcuts()
{
    if (Input::IsKeyDown(SDLK_LCTRL)) {
        if (Input::IsKeyPressed(SDLK_Q)) {
            SceneSerializer::SerializeScene(mScene, mCurrentScenePath);
            mWindow->Close();
        }
        if (Input::IsKeyPressed(SDLK_S)) {
            SceneSerializer::SerializeScene(mScene, mCurrentScenePath);
        }
    }
    if (Input::IsKeyPressed(SDLK_T)) {
        mMode = ImGuizmo::MODE::WORLD;
        mOperation = ImGuizmo::OPERATION::TRANSLATE;
    }
    if (Input::IsKeyPressed(SDLK_R)) {
        mMode = ImGuizmo::MODE::LOCAL;
        mOperation = ImGuizmo::OPERATION::ROTATE_SCREEN;
    }
    if (Input::IsKeyPressed(SDLK_B)) {
        mMode = ImGuizmo::MODE::WORLD;
        mOperation = ImGuizmo::OPERATION::SCALE;
    }
    if (Input::IsKeyPressed(SDLK_ESCAPE)) {
        mSelectedEntity = {};
    }
}
