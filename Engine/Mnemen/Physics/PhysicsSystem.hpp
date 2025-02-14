//
// > Notice: Amélie Heinrich @ 2025
// > Create Time: 2025-02-04 11:58:17
//

#pragma once

#include "World/Scene.hpp"

#include <iostream>

#include <Jolt/Jolt.h>

#include <Jolt/RegisterTypes.h>
#include <Jolt/Core/TempAllocator.h>
#include <Jolt/Core/Factory.h>
#include <Jolt/Core/JobSystemThreadPool.h>
#include <Jolt/Physics/PhysicsSettings.h>
#include <Jolt/Physics/PhysicsSystem.h>
#include <Jolt/Physics/Body/BodyInterface.h>
#include <Jolt/Physics/Collision/Shape/BoxShape.h>
#include <Jolt/Physics/Collision/Shape/SphereShape.h>
#include <Jolt/Physics/Body/BodyCreationSettings.h>
#include <Jolt/Physics/Body/BodyActivationListener.h>
#include <Jolt/Math/Vec3.h>

/// @brief A system for handling physics simulation in the application.
/// 
/// The `PhysicsSystem` class provides static methods for initializing, updating, and exiting 
/// the physics system. It processes physics updates based on the current state of the scene.
class PhysicsSystem
{
public:
    /// @brief Initializes the physics system.
    /// 
    /// This method sets up the necessary resources and configurations for the physics system.
    /// It must be called before any physics operations can take place.
    static void Init();

    /// @brief Exits the physics system and cleans up resources.
    /// 
    /// This method shuts down the physics system and releases any resources used during its 
    /// operation. It should be called when the system is no longer needed.
    static void Exit();

    /// @brief Updates the physics system based on the given scene and minimum step duration.
    /// 
    /// This method performs the physics simulation for the current frame, including collisions, 
    /// movement, and other physics-related operations. It ensures that the system progresses in 
    /// a consistent manner based on the scene's current state.
    /// 
    /// @param scene The scene object that provides the current state of entities for physics processing.
    /// @param minStepDuration The minimum duration (in seconds) of a physics simulation step.
    static void Update(Ref<Scene> scene, float minStepDuration);
private:

    
    
};

class ContactListener
{
public:
    
};
