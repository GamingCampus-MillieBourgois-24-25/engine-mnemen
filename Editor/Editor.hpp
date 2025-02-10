//
// > Notice: Amélie Heinrich @ 2025
// > Create Time: 2025-02-03 12:23:59
//

#pragma once

#include <Mnemen.hpp>
#include <imgui.h>

#include "EditorCamera.hpp"

class Editor : public Application
{
public:
    Editor(ApplicationSpecs specs);
    ~Editor();

    virtual void OnUpdate(float dt) override;
    virtual void OnPhysicsTick() override;
    virtual void OnImGui(const Frame& frame) override;
private:
    Camera mCamera;

    Entity* mCameraEntity;
    Entity* mSponza;

    ImVec2 mViewportSize;
    bool mViewportFocused = false;
};
