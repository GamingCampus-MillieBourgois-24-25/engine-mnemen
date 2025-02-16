//
// > Notice: Amélie Heinrich @ 2025
// > Create Time: 2025-02-03 22:13:36
//

#pragma once

#include "GraphicsPipeline.hpp"

#include <Agility/d3dx12/d3dx12.h>

/// @brief Represents a mesh pipeline state for handling mesh shaders.
class MeshPipeline
{
public:
    /// @brief Type alias for referencing
    using Ref = Ref<MeshPipeline>;

    /// @brief Creates a mesh pipeline with the specified device and specs.
    /// @param devicePtr The Direct3D 12 device.
    /// @param specs The pipeline specification for graphics pipeline creation.
    MeshPipeline(Device::Ref devicePtr, GraphicsPipelineSpecs& specs);

    /// @brief Destroys the mesh pipeline.
    ~MeshPipeline();

    /// @brief Retrieves the underlying Direct3D 12 pipeline state object.
    /// @return Pointer to the pipeline state object.
    ID3D12PipelineState* GetPipeline() { return mPipeline; }

    /// @brief Retrieves the root signature associated with this mesh pipeline.
    /// @return The root signature reference.
    RootSignature::Ref GetSignature() { return mSignature; }

private:
    ID3D12PipelineState* mPipeline = nullptr; ///< Direct3D 12 pipeline state object.
    RootSignature::Ref mSignature = nullptr; ///< Root signature for binding resources.
};
