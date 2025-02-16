//
// > Notice: Amélie Heinrich @ 2025
// > Create Time: 2025-12-03 18:58:51
//

#pragma once

#include <RHI/Device.hpp>

/// @brief Defines the possible root types in a Direct3D 12 root signature.
enum class RootType
{
    /// @brief Represents a push constant type.
    PushConstant = 0xFFFF,
    /// @brief Represents a constant buffer type.
    ConstantBuffer = D3D12_DESCRIPTOR_RANGE_TYPE_CBV,
    /// @brief Represents a shader resource (SRV) type.
    ShaderResource = D3D12_DESCRIPTOR_RANGE_TYPE_SRV,
    /// @brief Represents an unordered access view (UAV) type.
    Storage = D3D12_DESCRIPTOR_RANGE_TYPE_UAV,
    /// @brief Represents a sampler type.
    Sampler = D3D12_DESCRIPTOR_RANGE_TYPE_SAMPLER
};

/// @brief Represents a root signature in Direct3D 12, which describes the resources 
/// and how they are accessed in shaders.
class RootSignature
{
public:
    /// @brief Type alias for referencing
    using Ref = Ref<RootSignature>;

    /// @brief Constructs an empty root signature.
    /// @param device The device associated with this root signature.
    RootSignature(Device::Ref device);

    /// @brief Constructs a root signature with a specified list of root types and optional push constant size.
    /// @param device The device associated with this root signature.
    /// @param roots A list of root types defining the resources in the root signature.
    /// @param pushConstantSize The size of push constants, defaults to 0.
    RootSignature(Device::Ref device, const Vector<RootType>& roots, UInt64 pushConstantSize = 0);

    /// @brief Destructor that cleans up the root signature.
    ~RootSignature();

    /// @brief Returns the D3D12 root signature object.
    /// @return The D3D12 root signature object.
    ID3D12RootSignature* GetSignature() { return mRootSignature; }

private:
    ID3D12RootSignature* mRootSignature = nullptr; ///< The Direct3D 12 root signature object.
};
