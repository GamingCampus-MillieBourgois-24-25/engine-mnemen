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
        mAwake();
    }
}

void ScriptInstance::Update(float dt)
{
    if (mUpdate.valid()) {
        mUpdate(dt);
    }
}

void ScriptInstance::Quit()
{
    if (mQuit.valid()) {
        mQuit();
    }
}
