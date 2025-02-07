//
// > Notice: Amélie Heinrich @ 2025
// > Create Time: 2025-02-06 14:11:02
//

#pragma once

#include "Scene.hpp"

/// @brief Handles serialization and deserialization of scenes.
/// @details Provides functionality to save and load scenes from disk.
class SceneSerializer
{
public:
    /// @brief Serializes a scene and saves it to a file.
    /// @param scene The scene to serialize.
    /// @param path The file path where the scene will be saved.
    static void SerializeScene(Ref<Scene> scene, const String& path);

    /// @brief Deserializes a scene from a file.
    /// @param path The file path from which the scene will be loaded.
    /// @return A reference to the deserialized scene.
    static Ref<Scene> DeserializeScene(const String& path);
};
