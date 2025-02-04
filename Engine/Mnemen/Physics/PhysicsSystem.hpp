//
// > Notice: Amélie Heinrich @ 2025
// > Create Time: 2025-02-04 11:58:17
//

#pragma once

#include "World/Scene.hpp"

class PhysicsSystem
{
public:
    static void Init();
    static void Exit();

    static void Update(Scene& scene, float minStepDuration);
};
