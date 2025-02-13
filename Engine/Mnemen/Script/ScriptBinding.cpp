//
// > Notice: Amélie Heinrich @ 2025
// > Create Time: 2025-02-13 18:38:50
//

#include "ScriptBinding.hpp"
#include "ScriptWrapper.hpp"

void ScriptBinding::InitBindings(sol::state& state)
{
    InitEntity(state);
}

void ScriptBinding::InitEntity(sol::state& state)
{
    state["Entity"] = state.create_table();
    state["Entity"]["GetName"] = &LuaWrapper::LuaEntity::GetName;
}
