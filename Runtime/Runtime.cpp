//
// > Notice: Amélie Heinrich @ 2025
// > Create Time: 2025-02-03 12:26:03
//

#include "Runtime.hpp"

#include <imgui.h>

Runtime::Runtime(ApplicationSpecs specs)
    : Application(specs)
{
    mScenePlaying = true;

    mCameraEntity = mScene->AddEntity("Editor Camera");
    mCameraEntity->Private = true;
    
    auto& cam = mCameraEntity->AddComponent<CameraComponent>();
    cam.Primary = true;
    cam.FOV = 90.0f;
    cam.Near = 0.1f;
    cam.Far = 200.0f;
    cam.AspectRatio = (float)specs.Width / (float)specs.Height;
}

Runtime::~Runtime()
{

}

void Runtime::OnUpdate(float dt)
{
    
}

void Runtime::OnPhysicsTick()
{

}

void Runtime::OnImGui(const Frame& frame)
{
   
}
