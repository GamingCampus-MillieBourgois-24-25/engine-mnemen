//
// > Notice: Amélie Heinrich @ 2025
// > Create Time: 2025-02-04 11:59:14
//

#include "PhysicsSystem.hpp"

JPH::PhysicsSystem* sPhysicsSystem;
JPH::BodyInterface* sPhysicsWorld;
JPH::Vec3 sGravity;
JPH::TempAllocatorImpl* sAllocator;
constexpr JPH::BroadPhaseLayer sNonMoving(0) ;
constexpr JPH::BroadPhaseLayer sMoving(1);
constexpr int mLayers(2);
constexpr JPH::ObjectLayer sNonMovingLayer = 0;
constexpr JPH::ObjectLayer sMovingLayer = 1;
constexpr JPH::ObjectLayer sLayers = 2;

#include <Core/Logger.hpp>

bool ObjectLayerPairFilter::ShouldCollide(JPH::ObjectLayer ObjectLayer1, JPH::ObjectLayer ObjectLayer2)
{
    switch (ObjectLayer1)
    {
    case sNonMovingLayer:
        return ObjectLayer2 == sMovingLayer;
    case sMovingLayer:
        return true;
    default:
        LOG_INFO("Initialized Collision System");
        return false;
    }
}

BroadPhaseLayerInterface::BroadPhaseLayerInterface()
{
    //JPH::BroadPhaseLayer mObjectToBroadPhase[sNonMovingLayer] = sNonMoving;
}

void PhysicsSystem::Init()
{
    JPH::RegisterDefaultAllocator();

    JPH::Factory::sInstance = new JPH::Factory();

    JPH::RegisterTypes();

    int MaxBodies = 65536;
    int NumBodyMutexes = 0;
    int MaxBodyPairs = 65536;
    int MaxContactConstraints = 10240;

    /*JPH::BPLayerInterfaceImpl broad_phase_layer_interface;
    ObjectVsBroadPhaseLayerFilterImpl object_vs_broadphase_layer_filter;
    ObjectLayerPairFilterImpl object_vs_object_layer_filter;*/

    //Create the allocator
    sAllocator = new JPH::TempAllocatorImpl(1 * 1024 * 1024);

    //Initialize the Jolt Physics system
    sPhysicsSystem = new JPH::PhysicsSystem();
    //sPhysicsSystem->Init(MaxBodies, NumBodyMutexes, MaxBodyPairs, MaxContactConstraints, broad_phase_layer_interface, 
    //object_vs_broadphase_layer_filter, object_vs_object_layer_filter );
    sGravity = JPH::Vec3(0.0f, -9.81f, 0.0f);
    sPhysicsSystem->SetGravity(sGravity);

    //Create the physics world
    sPhysicsWorld = &sPhysicsSystem->GetBodyInterface();

    LOG_INFO("Initialized Physics System");
}

void PhysicsSystem::Exit()
{
    // Clean up allocated resources
    JPH::UnregisterTypes();
    delete JPH::Factory::sInstance;
    JPH::Factory::sInstance = nullptr;
    delete sPhysicsSystem;
    delete sAllocator;
}

void PhysicsSystem::Update(Ref<Scene> scene, float minStepDuration)
{
    
}
