//
// > Notice: Amélie Heinrich @ 2025
// > Create Time: 2025-02-03 12:26:03
//

#include "Editor.hpp"

#include <imgui.h>

Editor::Editor(ApplicationSpecs specs)
    : Application(specs)
{
    mCameraEntity = mScene->AddEntity("Editor Camera");
    mCameraEntity->Private = true;
    auto& cam = mCameraEntity->AddComponent<CameraComponent>();
    cam.Primary = true;

    mSponza = mScene->AddEntity("Sponza");

    auto& mesh = mSponza->AddComponent<MeshComponent>();
    mesh.Init("Assets/Models/Sponza/Sponza.gltf");

    auto& script = mSponza->AddComponent<ScriptComponent>();
    script.Load("Assets/Scripts/Hello.wren");
}

Editor::~Editor()
{

}

void Editor::OnUpdate(float dt)
{
    int width, height;
    mWindow->PollSize(width, height);

    mCamera.Update(dt, width, height);

    auto& cam = mCameraEntity->GetComponent<CameraComponent>();
    cam.Primary = true;
    cam.Projection = mCamera.Projection();
    cam.View = mCamera.View();
}

void Editor::OnPhysicsTick()
{

}

void Editor::OnImGui()
{
    ImGui::Begin("Debug Window");
    if (ImGui::Button("Save Test Scene")) {
        SceneSerializer::SerializeScene(mScene, "Assets/Scenes/Test.json");
    }
    ImGui::End();
}
