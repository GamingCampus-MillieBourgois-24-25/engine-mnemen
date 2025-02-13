//
// > Notice: Amélie Heinrich @ 2025
// > Create Time: 2025-02-04 22:12:49
//

#pragma once

#include <sol/sol.hpp>

#include "World/Scene.hpp"

class ScriptSystem
{
public:
    static void Init();
    static void Exit();

    static void Awake(Ref<Scene> scene);
    static void Update(Ref<Scene> scene, float dt);
    static void Quit(Ref<Scene> scene);

    static sol::state* GetState() { return &sData.State; }    
private:
    static void LogCallback(const sol::variadic_args& args);
    static int PanicCallback(lua_State* L);

    static struct Data {
        sol::state State;
    } sData;
};

