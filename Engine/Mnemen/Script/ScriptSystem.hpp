//
// > Notice: Amélie Heinrich @ 2025
// > Create Time: 2025-02-04 22:12:49
//

#pragma once

#include "World/Scene.hpp"

#include <wren.h>

class ScriptSystem
{
public:
    static void Init();
    static void Exit();

    static void Update(Scene& scene);
private:
    static struct Data {
        WrenVM* VirtualMachine;
    } sData;

    static void WriteCallback(WrenVM* vm, const char* text);
    static void ErrorCallback(WrenVM* vm, WrenErrorType errorType, const char* module, const int line, const char* msg);
};
