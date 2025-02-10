//
// > Notice: Amélie Heinrich @ 2025
// > Create Time: 2025-02-04 23:48:04
//

#pragma once

#include <Wren++/Wren++.h>
#include <Asset/AssetManager.hpp>

/// @brief Represents an individual script within the scripting system.
/// @details The `Script` class is responsible for managing the execution of a script,
/// including initialization, updates, and cleanup.
class Script
{
public:
    /// @brief Default constructor.
    Script() = default;

    /// @brief Default destructor.
    ~Script() = default;

    /// @brief Sets the script source from an asset handle.
    /// @param handle The asset handle pointing to the script source.
    bool SetSource(Asset::Handle handle);

    /// @brief Calls the "Awake" method of the script.
    /// @details This method is typically called when the script is first initialized.
    void Awake();

    /// @brief Calls the "Quit" method of the script.
    /// @details This method is called when the script is about to be removed or the scene is closing.
    void Quit();

    /// @brief Calls the "Update" method of the script.
    /// @param dt Delta time (time elapsed since the last frame).
    /// @details This method is called every frame to update the script's logic.
    void Update(float dt);

private:
    wrenpp::VM mVirtualMachine; ///< The Wren virtual machine instance managing the script execution.
    wrenpp::Method mAwake;      ///< Reference to the "Awake" method within the script.
    wrenpp::Method mQuit;       ///< Reference to the "Quit" method within the script.
    wrenpp::Method mUpdate;     ///< Reference to the "Update" method within the script.

    Asset::Handle mHandle;      ///< Handle to the script asset.
    bool mLoaded = false;       ///< Whether or not the script is loaded.
};

