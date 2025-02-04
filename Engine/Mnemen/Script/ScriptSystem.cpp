//
// > Notice: Amélie Heinrich @ 2025
// > Create Time: 2025-02-04 22:19:51
//

#include "ScriptSystem.hpp"

#include <Core/Logger.hpp>

ScriptSystem::Data ScriptSystem::sData;

void ScriptSystem::Init()
{
    WrenConfiguration config = {};
    config.writeFn = &ScriptSystem::WriteCallback;
    config.errorFn = &ScriptSystem::ErrorCallback;

    sData.VirtualMachine = wrenNewVM(&config);

    LOG_INFO("Initialized Script System");
}

void ScriptSystem::Exit()
{
    wrenFreeVM(sData.VirtualMachine);
}

void ScriptSystem::Update(Scene& scene)
{
    
}

void ScriptSystem::WriteCallback(WrenVM* vm, const char* text)
{
    LOG_INFO("[SCRIPT] {0}", text);
}

void ScriptSystem::ErrorCallback(WrenVM* vm, WrenErrorType errorType, const char* module, const int line, const char* msg)
{
    switch (errorType)
    {
        case WREN_ERROR_COMPILE: {
            LOG_ERROR("[{0} line {1}] [Error] {2}\n", module, line, msg);
        } break;
        case WREN_ERROR_STACK_TRACE: {
            LOG_ERROR("[{0} line {1}] in {2}\n", module, line, msg);
        } break;
        case WREN_ERROR_RUNTIME: {
            LOG_ERROR("[Runtime Error] %s\n", msg);
        } break;
    }
}
