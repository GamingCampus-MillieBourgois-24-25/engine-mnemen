//
// > Notice: Amélie Heinrich @ 2025
// > Create Time: 2025-02-04 22:12:49
//

#pragma once

#include "World/Scene.hpp"

#include <Wren++/Wren++.h>

class ScriptSystem
{
public:
    static void Init();
    static void Exit();

    static void Awake(Scene& scene);
    static void Update(Scene& scene, float dt);
    static void Quit(Scene& scene);
};
