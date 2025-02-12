//
// > Notice: Amélie Heinrich @ 2025
// > Create Time: 2025-02-04 22:19:51
//

#include "ScriptSystem.hpp"

#include <Core/Logger.hpp>
#include <Core/Profiler.hpp>

void ScriptSystem::Init()
{
    LOG_INFO("Initialized Script System");
}

void ScriptSystem::Exit()
{
    // Nothing to do, it auto cleans itself. Yay!!!
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
