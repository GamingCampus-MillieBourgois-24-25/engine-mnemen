//
// > Notice: Amélie Heinrich @ 2025
// > Create Time: 2025-02-13 16:28:50
//

#pragma once

#include "Script.hpp"

class ScriptInstance
{
public:
    ScriptInstance(sol::load_result* script);
    ~ScriptInstance() = default;

    void Reset(int entityID);
    void Awake();
    void Update(float dt);
    void Quit();
private:
    sol::load_result* mParent;

    sol::table mTable;
    sol::function mAwake;
    sol::function mUpdate;
    sol::function mQuit;
};
