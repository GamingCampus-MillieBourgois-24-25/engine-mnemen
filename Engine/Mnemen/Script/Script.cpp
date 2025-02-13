//
// > Notice: Amélie Heinrich @ 2025
// > Create Time: 2025-02-13 16:25:05
//

#include <Core/Logger.hpp>

#include "Script.hpp"
#include "ScriptSystem.hpp"

Script::Script(const String& path)
{
    sol::state* state = ScriptSystem::GetState();   
    
    mHandle = state->load_file(path);
    if (!mHandle.valid()) {
        mValid = false;
        LOG_ERROR("Failed to compile Lua script!");
        mHandle = {};
        return;
    }
    mValid = true;
}

Script::~Script()
{
    if (mHandle) {
        mHandle = {};
        ScriptSystem::GetState()->collect_garbage();
    }
}
