//
// > Notice: Amélie Heinrich @ 2025
// > Create Time: 2025-12-03 10:30:38
//

#pragma once

#include <RHI/Device.hpp>
#include <RHI/Resource.hpp>

/// @brief Enum representing different texture formats.
/// @details This enum is used to define the format of textures, specifying how pixel data is stored and interpreted.
///          It includes various formats for standard RGBA, depth, compressed textures, and floating-point formats.
enum class TextureFormat
{
    Unknown = DXGI_FORMAT_UNKNOWN, ///< Unknown texture format.
    RGBA8 = DXGI_FORMAT_R8G8B8A8_UNORM, ///< 8-bit per channel RGBA texture (unsigned normalized).
    RGBA8_sRGB = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB, ///< 8-bit per channel RGBA texture in sRGB space.
    RGBA16Float = DXGI_FORMAT_R16G16B16A16_FLOAT, ///< 16-bit floating point RGBA texture.
    RGBA32Float = DXGI_FORMAT_R32G32B32A32_FLOAT, ///< 32-bit floating point RGBA texture.
    RGB11Float = DXGI_FORMAT_R11G11B10_FLOAT, ///< RGB texture with 11 bits for RGB and 10 bits for the alpha channel.
    RG8 = DXGI_FORMAT_R8G8_UNORM, ///< 8-bit per channel RG texture.
    R8 = DXGI_FORMAT_R8_UNORM, ///< 8-bit single channel texture.
    BC7 = DXGI_FORMAT_BC7_UNORM, ///< BC7 compressed texture format.
    R32Float = DXGI_FORMAT_R32_FLOAT, ///< 32-bit floating point R texture.
    Depth32 = DXGI_FORMAT_D32_FLOAT ///< Depth texture with 32-bit floating point precision.
};

/// @brief Enum representing the different usages for textures.
/// @details This enum defines the types of operations a texture can be used for, such as render targets, depth targets,
///          storage resources, or shader resources. It uses bit flags to combine multiple usages.
enum class TextureUsage
{
    RenderTarget = BIT(1), ///< Texture can be used as a render target.
    DepthTarget = BIT(2), ///< Texture can be used as a depth target.
    Storage = BIT(3), ///< Texture can be used as a storage resource (UAV).
    ShaderResource = BIT(4) ///< Texture can be used as a shader resource (SRV).
};

/// @brief Structure that describes a texture's properties.
/// @details This structure contains the essential properties for creating and using a texture, such as its size,
///          format, and intended usage.
struct TextureDesc
{
    String Name; ///< The name of the texture.
    UInt32 Width; ///< The width of the texture in pixels.
    UInt32 Height; ///< The height of the texture in pixels.
    UInt32 Depth; ///< The depth of the texture (for 3D textures).
    UInt32 Levels; ///< The number of mipmap levels (0 for no mipmaps).
    TextureFormat Format; ///< The format of the texture (e.g., RGBA8, R32Float, etc.).
    TextureUsage Usage; ///< The intended usage of the texture (e.g., render target, shader resource, etc.).
};

/// @brief Class representing a texture resource.
/// @details This class encapsulates the creation and management of a texture resource, including its properties,
///          resource creation, and interaction with the GPU. It can be created either from an existing `ID3D12Resource`
///          or from scratch using a `TextureDesc` configuration.
/// @note Textures can be used as render targets, depth targets, or shader resources, depending on their usage flags.
class Texture : public Resource
{
public:
    /// @brief Type alias for referencing
    using Ref = Ref<Texture>;

    /// @brief Creates a texture from an existing `ID3D12Resource` and a `TextureDesc` configuration.
    /// @param device The device used to create the texture.
    /// @param resource The existing Direct3D texture resource.
    /// @param desc The descriptor that defines the texture's properties.
    Texture(Device::Ref device, ID3D12Resource* resource, TextureDesc desc);

    /// @brief Creates a texture based on a `TextureDesc` configuration, without an existing `ID3D12Resource`.
    /// @param device The device used to create the texture.
    /// @param desc The descriptor that defines the texture's properties.
    Texture(Device::Ref device, TextureDesc desc);

    /// @brief Destructor that cleans up the texture's resources.
    ~Texture();

    /// @brief Retrieves the texture descriptor that defines the texture's properties.
    /// @return The texture's descriptor.
    TextureDesc GetDesc() const { return mDesc; }

    /// @brief Converts a string representation of a texture format to its corresponding `TextureFormat` enum value.
    /// @param format The string representation of a texture format.
    /// @return The corresponding `TextureFormat` enum value.
    static TextureFormat StringToFormat(const String& format);

private:
    TextureDesc mDesc; ///< The descriptor containing the texture's properties.
};

inline constexpr bool operator&(TextureUsage x, TextureUsage y)
{
    return static_cast<UInt32>(x) & static_cast<UInt32>(y);
}

inline constexpr TextureUsage operator|(TextureUsage x, TextureUsage y)
{
    return static_cast<TextureUsage>(static_cast<UInt32>(x) | static_cast<UInt32>(y));
}

inline constexpr TextureUsage operator^(TextureUsage x, TextureUsage y)
{
    return static_cast<TextureUsage>(static_cast<UInt32>(x) ^ static_cast<UInt32>(y));
}

inline constexpr TextureUsage operator~(TextureUsage x)
{
    return static_cast<TextureUsage>(~static_cast<UInt32>(x));
}

inline TextureUsage& operator|=(TextureUsage & x, TextureUsage y)
{
    x = x | y;
    return x;
}

inline TextureUsage& operator^=(TextureUsage & x, TextureUsage y)
{
    x = x ^ y;
    return x;
}
