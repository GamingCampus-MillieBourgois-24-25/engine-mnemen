//
// > Notice: Amélie Heinrich @ 2025
// > Create Time: 2025-12-04 21:28:06
//

#pragma once

#include <Asset/AssetManager.hpp>
#include <Asset/Shader.hpp>
#include <Core/File.hpp>

#include <nvtt/nvtt.h>

/// @struct AssetFile
/// @brief Represents an asset file with metadata and data bytes.
///
/// The AssetFile structure contains a header with metadata information 
/// and the actual binary data of the asset.
struct AssetFile
{
    /// @struct Header
    /// @brief Metadata header for the asset file.
    ///
    /// Contains information about the file time, asset type, and additional 
    /// headers for specific asset types like textures and shaders.
    struct Header
    {
        File::Filetime Filetime; ///< The file timestamp.
        AssetType Type; ///< The type of asset.

        struct {
            int Width; ///< Width of the texture.
            int Height; ///< Height of the texture.
            int Levels; ///< Mipmap levels.
        } TextureHeader; ///< Header for texture assets.

        struct {
            ShaderType Type; ///< Type of shader.
        } ShaderHeader; ///< Header for shader assets.
    } Header; ///< The header of the asset.

    Vector<UInt8> Bytes; ///< The binary data of the asset.
};

/// @class AssetCacher
/// @brief Manages asset caching and retrieval.
///
/// Provides functionality to initialize the asset cache, store assets, 
/// and check if an asset is cached.
class AssetCacher
{
public:
    /// @brief Initializes the asset caching system.
    /// @param assetDirectory The directory where assets are stored.
    static void Init(const String& assetDirectory);

    /// @brief Caches an asset from the given file path.
    /// @param normalPath The path of the asset to cache.
    static void CacheAsset(const String& normalPath);

    /// @brief Checks if an asset is already cached.
    /// @param normalPath The path of the asset.
    /// @return True if the asset is cached, false otherwise.
    static bool IsCached(const String& normalPath);

    /// @brief Reads an asset file from disk.
    /// @param path The path to the asset file.
    /// @return The AssetFile object containing the asset data.
    static AssetFile ReadAsset(const String& path);

private:
    friend class AssetManager; ///< Allows AssetManager to access private members.

    /// @struct Data
    /// @brief Internal data structure for asset caching.
    static struct Data
    {
        nvtt::Context mContext; ///< The NVTT context for handling texture assets.
    } sData;

    /// @brief Reads the header of an asset file.
    /// @param path The path to the asset file.
    /// @return The AssetFile object containing only the header information.
    static AssetFile ReadAssetHeader(const String& path);

    /// @brief Retrieves the entry point from a shader type.
    /// @param type The shader type.
    /// @return The entry point as a string.
    static String GetEntryPointFromShaderType(ShaderType type);

    /// @brief Determines the shader type from the file path.
    /// @param path The file path of the shader.
    /// @return The corresponding ShaderType.
    static ShaderType GetShaderTypeFromPath(const String& path);

    /// @brief Determines the asset type from the file path.
    /// @param normalPath The file path of the asset.
    /// @return The corresponding AssetType.
    static AssetType GetAssetTypeFromPath(const String& normalPath);

    /// @brief Retrieves the cached asset file path.
    /// @param normalPath The file path of the original asset.
    /// @return The cached asset file path.
    static String GetCachedAsset(const String& normalPath);
};

