//
// > Notice: Amélie Heinrich @ 2025
// > Create Time: 2025-12-04 03:20:02
//

#pragma once

#include <RHI/Device.hpp>
#include <RHI/DescriptorHeap.hpp>

/// @brief Specifies the addressing mode for texture sampling.
enum class SamplerAddress
{
    /// @brief Wraps the texture coordinates. Repeats the texture if out of bounds.
    Wrap = D3D12_TEXTURE_ADDRESS_MODE_WRAP,
    /// @brief Mirrors the texture coordinates. Flips the texture at the edges.
    Mirror = D3D12_TEXTURE_ADDRESS_MODE_MIRROR,
    /// @brief Clamps the texture coordinates to the edge of the texture.
    Clamp = D3D12_TEXTURE_ADDRESS_MODE_CLAMP,
    /// @brief Uses a fixed border color when the texture coordinates are out of bounds.
    Border = D3D12_TEXTURE_ADDRESS_MODE_BORDER
};

/// @brief Specifies the filter mode for texture sampling.
enum class SamplerFilter
{
    /// @brief Performs linear interpolation between adjacent texture samples.
    Linear = D3D12_FILTER_MIN_MAG_MIP_LINEAR,
    /// @brief Uses the nearest neighbor sampling method without interpolation.
    Nearest = D3D12_FILTER_MIN_MAG_MIP_POINT,
    /// @brief Uses anisotropic filtering to improve quality at steep angles.
    Anistropic = D3D12_FILTER_ANISOTROPIC
};

/// @brief Represents a texture sampler in Direct3D 12, used to define how texture sampling is performed
/// when accessing a texture in shaders.
class Sampler
{
public:
    /// @brief Type alias for referencing
    using Ref = Ref<Sampler>;

    /// @brief Constructs a sampler with the specified device, descriptor heaps, addressing mode, filter type,
    ///        and additional options such as mip levels and anisotropy.
    /// @param device The device associated with the sampler.
    /// @param heaps The descriptor heaps used by the sampler.
    /// @param address The addressing mode for the sampler.
    /// @param filter The filter type for the sampler.
    /// @param mips Whether the sampler should use mipmaps (default is false).
    /// @param anisotropyLevel The level of anisotropic filtering to use (default is 1).
    /// @param comparison Whether the sampler should use comparison filtering (default is false).
    Sampler(Device::Ref device, DescriptorHeaps heaps, SamplerAddress address, SamplerFilter filter, bool mips = false, int anisotropyLevel = 1, bool comparison = false);

    /// @brief Destructor that cleans up the sampler resources.
    ~Sampler();

    /// @brief Returns the descriptor associated with the sampler.
    /// @return The descriptor of the sampler.
    DescriptorHeap::Descriptor GetDescriptor() { return mDescriptor; }

    /// @brief Returns the index of the sampler in a bindless array, used for bindless sampling.
    /// @return The index of the sampler.
    int BindlesssSampler() { return mDescriptor.Index; }

    /// @brief Returns the addressing mode used by the sampler.
    /// @return The addressing mode.
    SamplerAddress Address() { return mAddress; }

    /// @brief Returns the filter type used by the sampler.
    /// @return The filter type.
    SamplerFilter Filter() { return mFilter; }

    /// @brief Returns whether the sampler uses mipmaps.
    /// @return `true` if mipmaps are used, `false` otherwise.
    bool HasMips() { return mMips; }

    /// @brief Returns the anisotropic filtering level of the sampler.
    /// @return The anisotropic level.
    int AnisotropyLevel() { return mAnisotropyLevel; } 

private:
    SamplerAddress mAddress; ///< The addressing mode of the sampler.
    SamplerFilter mFilter; ///< The filtering mode of the sampler.
    bool mMips = false; ///< Whether the sampler uses mipmaps.
    int mAnisotropyLevel = 0; ///< The anisotropy filtering level of the sampler.

    DescriptorHeap::Descriptor mDescriptor; ///< The descriptor associated with the sampler.
    DescriptorHeaps mHeaps; ///< The descriptor heaps used by the sampler.
};
