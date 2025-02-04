//
// > Notice: Amélie Heinrich @ 2024
// > Create Time: 2025-02-03 12:26:03
//

#include "Editor.hpp"

Editor::Editor(ApplicationSpecs specs)
    : Application(specs)
{
    mCameraEntity = mScene.AddEntity("Editor Camera");
    
    auto& cam = mCameraEntity->AddComponent<CameraComponent>();
    cam.Primary = true;
}

Editor::~Editor()
{

}

void Editor::OnUpdate(float dt)
{
    mCamera.Update(dt, 1280, 720);

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

}
