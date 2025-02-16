//
// > Notice: Amélie Heinrich @ 2025
// > Create Time: 2025-12-18 23:04:48
//

#pragma once

#include <RHI/Device.hpp>
#include <RHI/RootSignature.hpp>

#include <Asset/Shader.hpp>

/// @brief Represents a compute pipeline in Direct3D 12.
class ComputePipeline
{
public:
    /// @brief Type alias for referencing
    using Ref = Ref<ComputePipeline>;

    /// @brief Creates a compute pipeline.
    /// @param device The GPU device.
    /// @param shader The compute shader.
    /// @param signature The root signature.
    ComputePipeline(Device::Ref device, Shader shader, RootSignature::Ref signature);
    
    /// @brief Destroys the compute pipeline.
    ~ComputePipeline();

    /// @brief Retrieves the D3D12 pipeline state object.
    /// @return Pointer to the pipeline state.
    ID3D12PipelineState* GetPipeline() { return mPipeline; }

    /// @brief Retrieves the root signature used by the pipeline.
    /// @return Reference to the root signature.
    RootSignature::Ref GetSignature() { return mSignature; }

private:
    ID3D12PipelineState* mPipeline = nullptr; ///< D3D12 pipeline state object for compute shaders.
    RootSignature::Ref mSignature = nullptr; ///< Root signature defining resource bindings for the pipeline.
};
