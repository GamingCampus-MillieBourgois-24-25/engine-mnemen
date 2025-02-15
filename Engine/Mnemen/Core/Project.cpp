//
// > Notice: Amélie Heinrich @ 2025
// > Create Time: 2025-02-15 20:25:39
//

#include "Project.hpp"

#include <Core/File.hpp>

void Project::Load(const String& path)
{
    nlohmann::json root = File::LoadJSON(path);
    
    // Load scene files
    if (root.contains("sceneFiles") && root["sceneFiles"].is_object()) {
        for (auto& [sceneName, scenePath] : root["sceneFiles"].items()) {
            SceneMap[sceneName] = scenePath;
        }
    }

    // Load basic project properties
    Name = root["name"];
    PathAbsolute = root["projectPath"];
    StartScenePathRelative = root["startScene"];
    StartSceneName = SceneMap[StartScenePathRelative];
    
    // Load settings
    if (root.contains("settings") && root["settings"].is_object()) {
        auto& settings = root["settings"];

        Settings.PhysicsRefreshRate = settings.value("physicsRefreshRate", 90.0f);

        String compressionFormat = settings.value("compressionFormat", "bc3");
        if (compressionFormat == "bc3")
            Settings.Format = CompressionFormat::BC3;
        else if (compressionFormat == "bc7")
            Settings.Format = CompressionFormat::BC7;
        else
            Settings.Format = CompressionFormat::BC3;
    }
}

void Project::Save(const String& path)
{
    nlohmann::json root;
    
    // Save basic project properties
    root["name"] = Name;
    root["projectPath"] = PathAbsolute;
    root["startScene"] = StartScenePathRelative;
    
    // Save scene files
    for (const auto& [sceneName, scenePath] : SceneMap) {
        root["sceneFiles"][sceneName] = scenePath;
    }
    
    // Save settings
    root["settings"]["physicsRefreshRate"] = Settings.PhysicsRefreshRate;
    root["settings"]["compressionFormat"] = (Settings.Format == CompressionFormat::BC7) ? "bc7" : "bc3";
    
    // Write to file
    File::WriteJSON(root, path);
}
