//
// > Notice: Amélie Heinrich @ 2025
// > Create Time: 2025-02-13 16:32:25
//

#include <Core/Logger.hpp>

#include "ScriptInstance.hpp"

ScriptInstance::ScriptInstance(sol::load_result* script)
    : mParent(script)
{
    
}

void ScriptInstance::Reset(int entityID)
{
    sol::protected_function scriptConstructor = mParent->get<sol::protected_function>();
    if (!scriptConstructor.valid()) {
        LOG_ERROR("ScriptConstructor is not valid!");
        return;
    }

    sol::protected_function_result result = scriptConstructor();
    if (!result.valid()) {
        LOG_ERROR("Failed to load script constructor!");
        return;
    }

    mTable = result.get<sol::function>()(entityID);
    if (!mTable.valid()) {
        LOG_ERROR("mTable is not valid after script call!");
        return;
    }
    if (!mTable.is<sol::table>()) {
        LOG_ERROR("mTable is not a table! Type: {0}", (int)mTable.get_type());
        return;
    }

    mAwake = mTable["awake"];
    mUpdate = mTable["update"];
    mQuit = mTable["quit"];
}

void ScriptInstance::Awake()
{
    if (mAwake.valid()) {
        sol::protected_function_result result = mAwake();
        if (!result.valid()) {
            sol::error err = result;
            LOG_ERROR("[LUA::ERROR::AWAKE] Error: {0}", err.what());
        }
    }
}

void ScriptInstance::Update(float dt)
{
    if (mUpdate.valid()) {
        sol::protected_function_result result = mUpdate(dt);
        if (!result.valid()) {
            sol::error err = result;
            LOG_ERROR("[LUA::ERROR::UPDATE] Error: {0}", err.what());
        }
    }
}

void ScriptInstance::Quit()
{
    if (mQuit.valid()) {
        sol::protected_function_result result = mQuit();
        if (!result.valid()) {
            sol::error err = result;
            LOG_ERROR("[LUA::ERROR::UPDATE] Error: {0}", err.what());
        }
    }
}
