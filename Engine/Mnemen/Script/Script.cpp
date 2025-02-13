//
// > Notice: Amélie Heinrich @ 2025
// > Create Time: 2025-02-13 16:25:05
//

#include <Core/Logger.hpp>

#include "Script.hpp"
#include "ScriptSystem.hpp"

Script::Script(const String& path)
    : mPath(path)
{
    sol::state* state = ScriptSystem::GetState();   
    
    mHandle = state->load_file(path);
    if (!mHandle.valid()) {
        mValid = false;

        sol::error err = mHandle;
        LOG_ERROR("Failed to load Lua script: {0}", err.what());
        return;
    }
    mValid = true;
}

Script::~Script()
{
    if (mValid) {
        ScriptSystem::GetState()->collect_garbage();
        mValid = false;
    }
}

void Script::Reload()
{
    if (mValid) {
        ScriptSystem::GetState()->collect_garbage();
        mValid = false;
    }

    sol::state* state = ScriptSystem::GetState();   
    
    mHandle = state->load_file(mPath);
    if (!mHandle.valid()) {
        mValid = false;

        sol::error err = mHandle;
        LOG_ERROR("Failed to load Lua script: {0}", err.what());
        return;
    }
    mValid = true;
}
