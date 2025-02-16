//
// > Notice: Amélie Heinrich @ 2025
// > Create Time: 2025-12-12 04:40:23
//

#pragma once

#include <RHI/Device.hpp>
#include <RHI/DescriptorHeap.hpp>
#include <RHI/Resource.hpp>
#include <RHI/Buffer.hpp>

/// @brief A class representing a ray-tracing acceleration structure resource.
///
/// The `AccelerationStructure` class is a resource used in ray-tracing pipelines to speed up the
/// process of intersecting rays with geometry. It encapsulates the creation, management, and 
/// scratch memory associated with the acceleration structure, providing an efficient way to
/// handle ray tracing operations in a graphics API like DirectX 12.
class AccelerationStructure : public Resource
{
public:
    /// @brief Constructs an AccelerationStructure with the specified device and descriptor heaps.
    /// 
    /// This constructor initializes an acceleration structure resource, responsible for managing 
    /// the memory and data required for ray tracing. It takes a device reference to handle resource 
    /// creation and descriptor heaps for managing shader resources.
    /// 
    /// @param device The device reference used for resource creation.
    /// @param heaps Descriptor heaps used for managing ray-tracing resources.
    AccelerationStructure(Device::Ref device, DescriptorHeaps& heaps);

    /// @brief Destructor for the AccelerationStructure class.
    /// 
    /// The destructor is defaulted and ensures proper cleanup of resources associated with the 
    /// acceleration structure, including freeing any allocated memory or resources.
    ~AccelerationStructure() = default;

    /// @brief Frees the scratch buffer memory.
    /// 
    /// This method is used to release any memory used for scratch buffers during the creation or 
    /// updating of the acceleration structure. It is typically called once the structure is no 
    /// longer needed for updates.
    void FreeScratch();

    /// @brief Gets the scratch buffer associated with the acceleration structure.
    /// 
    /// This method returns the scratch buffer used to store temporary data during the building or 
    /// updating of the acceleration structure. It is a reference to a `Buffer` object.
    /// 
    /// @return The scratch buffer reference associated with the acceleration structure.
    Buffer::Ref GetScratch() { return mScratch; }

protected:
    friend class Uploader; ///< Allows `Uploader` class to access private members of `AccelerationStructure`.
    friend class CommandBuffer; ///< Allows `CommandBuffer` class to access private members of `AccelerationStructure`.

    /// @brief Allocates resources for the acceleration structure.
    /// 
    /// This method allocates memory and resources required for the acceleration structure, and
    /// optionally accepts a name for identification. It is typically used during the creation or 
    /// update process of the acceleration structure.
    /// 
    /// @param name The name used to identify the acceleration structure, defaulting to "Acceleration Structure".
    void Allocate(const String& name = "Acceleration Structure");

    DescriptorHeaps mHeaps; ///< Descriptor heaps used for managing ray-tracing resources.
    D3D12_BUILD_RAYTRACING_ACCELERATION_STRUCTURE_INPUTS mInputs; ///< Inputs used for building the acceleration structure.
    UInt64 mUpdateSize = 0; ///< The update size for the acceleration structure.
    Buffer::Ref mScratch = nullptr; ///< Scratch buffer used during acceleration structure updates.
};

