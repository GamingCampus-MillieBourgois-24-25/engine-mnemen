//
// > Notice: Amélie Heinrich @ 2025
// > Create Time: 2025-12-03 19:11:45
//

#pragma once

#include <RHI/RootSignature.hpp>
#include <RHI/Texture.hpp>
#include <Asset/Shader.hpp>

/// @brief Defines the fill mode for rasterization.
enum class FillMode
{
    Solid = D3D12_FILL_MODE_SOLID, ///< Solid fill mode.
    Line = D3D12_FILL_MODE_WIREFRAME ///< Wireframe mode.
};

/// @brief Defines the culling mode for rasterization.
enum class CullMode
{
    Back = D3D12_CULL_MODE_BACK, ///< Cull back-facing polygons.
    Front = D3D12_CULL_MODE_FRONT, ///< Cull front-facing polygons.
    None = D3D12_CULL_MODE_NONE ///< No culling.
};

/// @brief Defines depth comparison functions for depth testing.
enum class DepthOperation
{
    Greater = D3D12_COMPARISON_FUNC_GREATER, ///< Pass if fragment depth is greater.
    Less = D3D12_COMPARISON_FUNC_LESS, ///< Pass if fragment depth is less.
    Equal = D3D12_COMPARISON_FUNC_EQUAL, ///< Pass if fragment depth is equal.
    LEqual = D3D12_COMPARISON_FUNC_LESS_EQUAL, ///< Pass if fragment depth is less or equal.
    None = D3D12_COMPARISON_FUNC_NONE ///< No depth testing.
};

/// @brief Specifies the configuration for a graphics pipeline.
struct GraphicsPipelineSpecs
{
    FillMode Fill = FillMode::Solid; ///< Fill mode for rasterization.
    CullMode Cull = CullMode::None; ///< Culling mode.
    bool CCW = true; ///< Counter-clockwise front face.
    bool Line = false; ///< Enable line rendering.
    bool UseAmplification = false; ///< Use mesh amplification shaders.

    Vector<TextureFormat> Formats; ///< Render target formats.
    DepthOperation Depth = DepthOperation::None; ///< Depth test function.
    TextureFormat DepthFormat = TextureFormat::Unknown; ///< Depth buffer format.
    bool DepthEnabled = false; ///< Enable depth testing.
    bool DepthClampEnable = false; ///< Enable depth clamping.
    bool DepthWrite = true; ///< Enable writing to depth buffer.

    UnorderedMap<ShaderType, Shader> Bytecodes; ///< Shader bytecode collection.
    RootSignature::Ref Signature = nullptr; ///< Root signature for pipeline.
};

/// @brief Represents a graphics pipeline state.
class GraphicsPipeline
{
public:
    /// @brief Type alias for referencing
    using Ref = Ref<GraphicsPipeline>;

    /// @brief Creates a graphics pipeline.
    /// @param device The Direct3D 12 device.
    /// @param specs The pipeline specification.
    GraphicsPipeline(Device::Ref device, GraphicsPipelineSpecs& specs);

    /// @brief Destroys the graphics pipeline.
    ~GraphicsPipeline();

    /// @brief Retrieves the underlying Direct3D 12 pipeline state.
    /// @return Pointer to the pipeline state object.
    ID3D12PipelineState* GetPipeline() { return mPipeline; }

    /// @brief Retrieves the root signature associated with this pipeline.
    /// @return The root signature reference.
    RootSignature::Ref GetRootSignature() { return mSignature; }

private:
    ID3D12PipelineState* mPipeline = nullptr; ///< Direct3D 12 pipeline state object.
    RootSignature::Ref mSignature = nullptr; ///< Root signature for binding resources.
};
