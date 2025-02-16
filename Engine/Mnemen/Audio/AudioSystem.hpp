//
// > Notice: Amélie Heinrich @ 2025
// > Create Time: 2025-02-04 12:02:32
//

#pragma once

#include <miniaudio.h>

#include "World/Scene.hpp"

class AudioSystem
{
public:
    static void Init();
    static void Exit();

    static void Awake(Ref<Scene> scene);
    static void Update(Ref<Scene> scene);
    static void Quit(Ref<Scene> scene);

    static ma_engine* GetEngine() { return &sData.Engine; }
private:
    static struct Data {
        ma_device Device;
        ma_engine Engine;
    } sData;
};
