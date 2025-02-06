//
// > Notice: Amélie Heinrich @ 2025
// > Create Time: 2025-02-06 14:11:02
//

#pragma once

#include "Scene.hpp"

class SceneSerializer
{
public:
    static void SerializeScene(Ref<Scene> scene, const String& path);
    static Ref<Scene> DeserializeScene(const String& path);
};
