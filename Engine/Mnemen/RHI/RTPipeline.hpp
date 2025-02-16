//
// > Notice: Amélie Heinrich @ 2025
// > Create Time: 2025-12-18 22:48:18
//

#pragma once

#include <Asset/Shader.hpp>
#include <RHI/RootSignature.hpp>
#include <RHI/Buffer.hpp>

/// @brief Specifies the configuration parameters for a ray tracing pipeline.
struct RTPipelineSpecs
{
    /// @brief The payload size for the ray tracing pipeline, in bytes.
    UInt32 PayloadSize;
    /// @brief The attribute size for the ray tracing pipeline, default is 8 bytes.
    UInt32 AttribSize = 8;
    /// @brief The maximum recursion depth for the ray tracing pipeline, default is 3.
    UInt32 MaxRecursion = 3;
    /// @brief The shader library containing the ray tracing shaders.
    Shader Library;
    /// @brief The root signature associated with the ray tracing pipeline.
    RootSignature::Ref Signature = nullptr;
};

/// @brief Represents a ray tracing pipeline in Direct3D 12, used to manage ray tracing 
/// state objects, shaders, and root signatures.
class RTPipeline
{
public:
    /// @brief Type alias for referencing
    using Ref = Ref<RTPipeline>;

    /// @brief Constructs a ray tracing pipeline with the specified device, descriptor heaps, and configuration specs.
    /// @param device The device associated with this ray tracing pipeline.
    /// @param heaps The descriptor heaps used by the pipeline.
    /// @param specs The configuration specifications for the pipeline (e.g., payload size, recursion depth).
    RTPipeline(Device::Ref device, DescriptorHeaps& heaps, RTPipelineSpecs specs);

    /// @brief Destructor that cleans up the ray tracing pipeline resources.
    ~RTPipeline();

    /// @brief Returns the Direct3D 12 state object representing the ray tracing pipeline.
    /// @return The D3D12 state object representing the pipeline.
    ID3D12StateObject* GetPipeline() { return mPipeline; }

    /// @brief Returns the root signature associated with the ray tracing pipeline.
    /// @return The root signature of the ray tracing pipeline.
    RootSignature::Ref GetSignature() { return mSignature; }

    /// @brief Returns the buffer containing the intersection data used by the ray tracing pipeline.
    /// @return The buffer containing the intersection data.
    Buffer::Ref GetTables() { return mIDBuffer; }

private:
    ID3D12StateObject* mPipeline = nullptr; ///< The Direct3D 12 state object representing the pipeline.
    RootSignature::Ref mSignature = nullptr; ///< The root signature associated with the pipeline.
    Buffer::Ref mIDBuffer = nullptr; ///< The buffer containing the intersection data.
};
