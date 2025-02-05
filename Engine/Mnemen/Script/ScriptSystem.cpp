//
// > Notice: Amélie Heinrich @ 2025
// > Create Time: 2025-02-04 22:19:51
//

#include "ScriptSystem.hpp"

#include <Core/Logger.hpp>
#include <Core/Profiler.hpp>

void ScriptSystem::Init()
{
    wrenpp::VM::writeFn = [](const char* text) {
        LOG_INFO("[SCRIPT_LOG] {0}", text);
    };
    wrenpp::VM::errorFn = [](WrenErrorType type, const char* module_name, int line, const char* message) {
        LOG_ERROR("WREN ERROR: [{0} at {1}] - {2}", module_name, line, message);
    };

    LOG_INFO("Initialized Script System");
}

void ScriptSystem::Exit()
{
    // Nothing to do, it auto cleans itself. Yay!!!
}

void ScriptSystem::Awake(Scene& scene)
{
    auto registry = scene.GetRegistry();
    auto view = registry->view<ScriptComponent>();
    
    for (auto [entity, script] : view.each()) {
        script.Handle.Awake();
    }
}

void ScriptSystem::Update(Scene& scene, float dt)
{
    PROFILE_FUNCTION();

    auto registry = scene.GetRegistry();
    auto view = registry->view<ScriptComponent>();
    
    for (auto [entity, script] : view.each()) {
        script.Handle.Update(dt);
    }
}

void ScriptSystem::Quit(Scene& scene)
{
    auto registry = scene.GetRegistry();
    auto view = registry->view<ScriptComponent>();
    
    for (auto [entity, script] : view.each()) {
        script.Handle.Quit();
    }
}
