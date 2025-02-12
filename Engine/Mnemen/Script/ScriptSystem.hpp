//
// > Notice: Amélie Heinrich @ 2025
// > Create Time: 2025-02-04 22:12:49
//

#pragma once

#include "World/Scene.hpp"

#include <sol/sol.hpp>

/// @brief Manages script execution within a scene.
/// @details The `ScriptSystem` is responsible for initializing, updating, and shutting down scripts attached to entities in a scene.
class ScriptSystem
{
public:
    /// @brief Initializes the scripting system.
    /// @details This function sets up necessary resources for script execution.
    static void Init();

    /// @brief Shuts down the scripting system.
    /// @details This function releases any allocated resources related to script execution.
    static void Exit();

    /// @brief Calls the "Awake" method for all scripts in the given scene.
    /// @param scene The scene containing the scripts.
    /// @details This is typically called once when a scene is first loaded.
    static void Awake(Ref<Scene> scene);

    /// @brief Updates all scripts in the given scene.
    /// @param scene The scene containing the scripts.
    /// @param dt Delta time (time elapsed since the last frame).
    /// @details This function is called every frame to update script behavior.
    static void Update(Ref<Scene> scene, float dt);

    /// @brief Calls the "Quit" method for all scripts in the given scene.
    /// @param scene The scene containing the scripts.
    /// @details This is called when the scene is being unloaded or the application is shutting down.
    static void Quit(Ref<Scene> scene);
private:
    static struct Data {
        sol::state State; ///< The Lua virtual machine
    } sData; ///< The static data of the script system
};

