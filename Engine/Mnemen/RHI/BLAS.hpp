//
// > Notice: Amélie Heinrich @ 2025
// > Create Time: 2025-12-12 04:54:59
//

#pragma once

#include <RHI/AccelerationStructure.hpp>

/// @brief A class representing a Bottom-Level Acceleration Structure (BLAS) for ray tracing.
///
/// A Bottom-Level Acceleration Structure (BLAS) represents the geometric data in a ray-tracing
/// pipeline. It contains all the geometry information required to perform ray intersection tests 
/// and is typically constructed from vertex and index buffers. BLASes are used to build top-level 
/// acceleration structures (TLAS), which contain instances of BLASes for ray tracing in a scene.
class BLAS : public AccelerationStructure
{
public:
    /// @brief Type alias for a BLAS pointer
    using Ref = Ref<BLAS>;

    /// @brief Constructs a Bottom-Level Acceleration Structure (BLAS) with the given device, descriptor heaps, vertex buffer, index buffer, vertex count, and index count.
    /// 
    /// This constructor initializes a BLAS resource with the provided geometry data (vertex and index buffers)
    /// and creates the necessary acceleration structure for ray tracing operations. The structure will be named 
    /// using the provided name or default to "BLAS".
    /// 
    /// @param device The device reference used for resource creation.
    /// @param heaps Descriptor heaps used for managing ray-tracing resources.
    /// @param vertex The reference to the vertex buffer containing geometry data.
    /// @param index The reference to the index buffer containing geometry data.
    /// @param vtxCount The number of vertices in the vertex buffer.
    /// @param idxCount The number of indices in the index buffer.
    /// @param name The name to identify the BLAS (default is "BLAS").
    BLAS(Device::Ref device, DescriptorHeaps& heaps, Buffer::Ref vertex, Buffer::Ref index, UInt32 vtxCount, UInt32 idxCount, const String& name = "BLAS");

    /// @brief Destructor for the BLAS class.
    /// 
    /// The destructor is defaulted and ensures proper cleanup of any resources associated with the 
    /// Bottom-Level Acceleration Structure (BLAS).
    ~BLAS() = default;

private:
    D3D12_RAYTRACING_GEOMETRY_DESC mGeometryDesc; ///< Describes the geometry used in the BLAS.
};

