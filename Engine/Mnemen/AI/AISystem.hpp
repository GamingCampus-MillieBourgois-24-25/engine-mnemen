//
// > Notice: Amélie Heinrich @ 2025
// > Create Time: 2025-02-04 12:02:32
//

#pragma once

#include "World/Scene.hpp"

class AISystem
{
public:
    static void Init();
    static void Exit();

    static void Update(Scene& scene);
};
