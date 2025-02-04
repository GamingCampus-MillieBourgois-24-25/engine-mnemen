//
// > Notice: Amélie Heinrich @ 2025
// > Create Time: 2025-02-04 12:02:32
//

#pragma once

#include "World/Scene.hpp"

/// @class AISystem
/// @brief A system responsible for AI-related processing.
///
/// The AISystem class provides static methods for initializing, exiting,
/// and updating AI-related functionalities within a scene.
class AISystem
{
public:
    /// @brief Initializes the AI system.
    static void Init();
    
    /// @brief Shuts down the AI system.
    static void Exit();

    /// @brief Updates the AI system for the given scene.
    /// 
    /// @param scene The scene to update AI components in.
    static void Update(Scene& scene);
};
