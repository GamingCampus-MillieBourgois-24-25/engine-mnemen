//
// > Notice: Amélie Heinrich @ 2025
// > Create Time: 2025-02-04 22:19:51
//

#include "ScriptSystem.hpp"
#include "ScriptWrapper.hpp"
#include "ScriptBinding.hpp"

#include <sstream>

#include <Core/Logger.hpp>
#include <Core/Profiler.hpp>

ScriptSystem::Data ScriptSystem::sData;

void ScriptSystem::LogCallback(const sol::variadic_args& args)
{
    std::stringstream ss;
        
    for (int i = 0; i < args.size(); i++) {
        if (i > 0)
            ss << " ";
        if (args[i].is<std::string>()) {
            ss << args[i].as<std::string>();
        } else if (args[i].is<int>()) {
            ss << args[i].as<int>();
        } else if (args[i].is<double>()) {
            ss << args[i].as<double>();
        } else if (args[i].is<bool>()) {
            ss << (args[i].as<bool>() ? "true" : "false");
        }
    }
    LOG_INFO("[LUA] {0}", ss.str());
}

int ScriptSystem::PanicCallback(lua_State* L)
{
    const char* errorMsg = lua_tostring(L, -1);
    LOG_ERROR("[LUA::ERROR] {0}", errorMsg);
    return 0;
}

void ScriptSystem::Init()
{
    sData.State.open_libraries(sol::lib::base,
                               sol::lib::math,
                               sol::lib::os,
                               sol::lib::string,
                               sol::lib::table,
                               sol::lib::utf8,
                               sol::lib::io,
                               sol::lib::jit);
    sData.State.set_function("print", &ScriptSystem::LogCallback);
    sData.State.set_panic(&ScriptSystem::PanicCallback);

    ScriptBinding::InitBindings(sData.State);

    LOG_INFO("Initialized Script System");
}

void ScriptSystem::Exit()
{
    
}

void ScriptSystem::Awake(Ref<Scene> scene)
{
    entt::registry* reg = scene->GetRegistry();

    auto view = reg->view<ScriptComponent>();
    for (auto [id, script] : view.each()) {
        for (auto& instance : script.Instances) {
            instance->Instance->Reset((int)id);
            instance->Instance->Awake();
        }
    }
}

void ScriptSystem::Update(Ref<Scene> scene, float dt)
{
    PROFILE_FUNCTION();

    entt::registry* reg = scene->GetRegistry();

    auto view = reg->view<ScriptComponent>();
    for (auto [id, script] : view.each()) {
        for (auto& instance : script.Instances) {
            instance->Instance->Update(dt);
        }
    }
}

void ScriptSystem::Quit(Ref<Scene> scene)
{
    entt::registry* reg = scene->GetRegistry();

    auto view = reg->view<ScriptComponent>();
    for (auto [id, script] : view.each()) {
        for (auto& instance : script.Instances) {
            instance->Instance->Quit();
        }
    }
}
