//
// > Notice: Amélie Heinrich @ 2025
// > Create Time: 2025-02-15 20:21:18
//

#pragma once

#include <Core/Common.hpp>

enum class CompressionFormat
{
    BC3,
    BC7
};

struct ProjectSettings
{
    CompressionFormat Format;
};

struct Project
{
    String Name;
    String PathAbsolute;
    String StartScenePathRelative;
    String StartSceneName;
    UnorderedMap<String, String> SceneMap;

    // Settings
    ProjectSettings Settings;

    void Load(const String& path);
    void Save(const String& path);
};
