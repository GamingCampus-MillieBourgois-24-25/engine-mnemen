//
// > Notice: Amélie Heinrich @ 2025
// > Create Time: 2025-02-04 11:59:14
//

#include "PhysicsSystem.hpp"

JPH::PhysicsSystem* sPhysicsSystem;
JPH::BodyInterface* sPhysicsWorld;
JPH::Vec3 sGravity;
JPH::TempAllocatorImpl* sAllocator;

#include <Core/Logger.hpp>

void PhysicsSystem::Init()
{
    JPH::RegisterDefaultAllocator();

    Factory::sInstance = new Factory();

    int cMaxBodies = 65536;
    int cNumBodyMutexes = 0;
    int cMaxBodyPairs = 65536;
    int cMaxContactConstraints = 10240;

    /*JPH::BPLayerInterfaceImpl broad_phase_layer_interface;
    ObjectVsBroadPhaseLayerFilterImpl object_vs_broadphase_layer_filter;
    ObjectLayerPairFilterImpl object_vs_object_layer_filter;*/

    //Create the allocator
    sAllocator = new JPH::TempAllocatorImpl(1 * 1024 * 1024);

    //Initialize the Jolt Physics system
    sPhysicsSystem = new JPH::PhysicsSystem();
    //sPhysicsSystem->Init(65536, 0, 65536, 10240,broad_phase_layer_interface, object_vs_broadphase_layer_filter, object_vs_object_layer_filter );

    //Set up gravity (for example, standard gravity)
    sGravity = JPH::Vec3(0.0f, -9.81f, 0.0f);
    sPhysicsSystem->SetGravity(sGravity);

    //Create the physics world
    sPhysicsWorld = &sPhysicsSystem->GetBodyInterface();

    LOG_INFO("Initialized Physics System");
}

void PhysicsSystem::Exit()
{
    // Clean up allocated resources
    delete Factory::sInstance;
    Factory::sInstance = nullptr;
    delete sPhysicsSystem;
    delete sAllocator;
}

void PhysicsSystem::Update(Ref<Scene> scene, float minStepDuration)
{
    
}
