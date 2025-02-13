//
// > Notice: Amélie Heinrich @ 2025
// > Create Time: 2025-02-13 18:38:02
//

#pragma once

#include <sol/sol.hpp>

class ScriptBinding
{
public:
    static void InitBindings(sol::state& state);

private:
    static void InitKeycode(sol::state& state);
    static void InitEntity(sol::state& state);
    static void InitVec(sol::state& state);
    static void InitQuat(sol::state& state);
    static void InitInput(sol::state& state);
    static void InitTransform(sol::state& state);
};
