//
// > Notice: Amélie Heinrich @ 2025
// > Create Time: 2025-02-04 12:02:32
//

#pragma once

#include "World/Scene.hpp"

/// @brief A system for managing audio in the application.
/// 
/// The `AudioSystem` class provides static methods for initializing, shutting down, and updating
/// audio-related functionality within the application. It processes audio updates based on the 
/// current state of the scene.
class AudioSystem
{
public:
    /// @brief Initializes the audio system.
    /// 
    /// This method sets up the necessary resources for the audio system, like configuring the audio playback system.
    static void Init();

    /// @brief Shuts down the audio system.
    /// 
    /// This method releases any resources used by the audio system, stopping any ongoing audio
    /// playback and cleaning up.
    static void Exit();

    /// @brief Updates the audio system based on the current scene.
    /// 
    /// This method processes any audio updates needed based on the current state of the 
    /// given scene. It can be called every frame to keep audio in sync with game events.
    /// 
    /// @param scene The scene object that provides the current state for audio processing.
    static void Update(Scene& scene);
};
