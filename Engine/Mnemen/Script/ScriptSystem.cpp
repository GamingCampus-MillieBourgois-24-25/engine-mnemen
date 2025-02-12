//
// > Notice: Amélie Heinrich @ 2025
// > Create Time: 2025-02-04 22:19:51
//

#include "ScriptSystem.hpp"

#include <Core/Logger.hpp>
#include <Core/Profiler.hpp>

ScriptSystem::Data ScriptSystem::sData;

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

    LOG_INFO("Initialized Script System");
}

void ScriptSystem::Exit()
{
    
}

void ScriptSystem::Awake(Ref<Scene> scene)
{
    auto registry = scene->GetRegistry();
    auto view = registry->view<ScriptComponent>();
    
    for (auto [entity, script] : view.each()) {
        for (auto instance : script.Instances) {
            instance->Handle.Awake();
        }
    }
}

void ScriptSystem::Update(Ref<Scene> scene, float dt)
{
    PROFILE_FUNCTION();

    auto registry = scene->GetRegistry();
    auto view = registry->view<ScriptComponent>();
    
    for (auto [entity, script] : view.each()) {
        for (auto instance : script.Instances) {
            instance->Handle.Update(dt);
        }
    }
}

void ScriptSystem::Quit(Ref<Scene> scene)
{
    auto registry = scene->GetRegistry();
    auto view = registry->view<ScriptComponent>();
    
    for (auto [entity, script] : view.each()) {
        for (auto instance : script.Instances) {
            instance->Handle.Quit();
        }
    }
}
