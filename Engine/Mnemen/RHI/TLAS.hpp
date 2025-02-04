//
// > Notice: Amélie Heinrich @ 2025
// > Create Time: 2025-12-12 05:05:49
//

#pragma once

#include <RHI/AccelerationStructure.hpp>
#include <glm/glm.hpp>

/// @brief Structure representing a single instance in a Raytracing Acceleration Structure (TLAS).
/// @details This structure is used to define a single instance of a geometry in a top-level acceleration structure (TLAS) for ray tracing.
///          It contains transformation data, instance-specific parameters, and the reference to the acceleration structure (BLAS).
///          Instances in a TLAS represent objects in the scene that can be traced by the ray tracing pipeline.
struct RaytracingInstance
{
    glm::mat3x4 Transform; ///< 3x4 matrix representing the transformation of the instance (model to world).
    uint32_t InstanceID : 24; ///< Unique identifier for the instance (24-bit).
    uint32_t InstanceMask : 8; ///< Mask for the instance, used to control visibility during ray traversal (8-bit).
    uint32_t InstanceContributionToHitGroupIndex : 24; ///< Contribution index for the hit group this instance belongs to (24-bit).
    uint32_t Flags : 8; ///< Flags that provide additional information about the instance (e.g., visibility or shadow options).
    uint64_t AccelerationStructure; ///< GPU virtual address of the acceleration structure this instance refers to (64-bit).
};

/// @brief Class representing a Top-Level Acceleration Structure (TLAS) for ray tracing.
/// @details A TLAS is used to hold the instances of geometries that will be traced in a ray tracing operation.
///          It is built from a collection of instances, where each instance contains a transformation matrix,
///          a reference to a bottom-level acceleration structure (BLAS), and other instance-specific parameters.
class TLAS : public AccelerationStructure
{
public:
    /// @brief Type alias for referencing
    using Ref = Ref<TLAS>;

    /// @brief Constructs a TLAS using the provided instance buffer and other parameters.
    /// @param device The device used to create the TLAS.
    /// @param heaps The descriptor heaps used for SRV management.
    /// @param instanceBuffer The buffer containing the raytracing instances (e.g., `RaytracingInstance` array).
    /// @param numInstance The number of instances in the TLAS.
    /// @param name Optional name for the TLAS, used for debugging.
    TLAS(Device::Ref device, DescriptorHeaps& heaps, Buffer::Ref instanceBuffer, UInt32 numInstance, const String& name = "TLAS");

    /// @brief Destructor that cleans up the resources associated with the TLAS.
    ~TLAS();

    /// @brief Retrieves the bindless index for the TLAS SRV (Shader Resource View).
    /// @return The bindless index used to access the TLAS in shaders.
    int Bindless() const { return mSRV.Index; }

private:
    DescriptorHeap::Descriptor mSRV; ///< The descriptor for the TLAS SRV, allowing access to the TLAS from shaders.
};
