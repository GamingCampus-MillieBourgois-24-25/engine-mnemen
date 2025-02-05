//
// > Notice: Amélie Heinrich @ 2025
// > Create Time: 2025-12-04 20:57:18
//

#pragma once

#include <Asset/Shader.hpp>
#include <Asset/Image.hpp>
#include <Asset/Mesh.hpp>
#include <Asset/ScriptSource.hpp>

#include <RHI/RHI.hpp>

/// @enum AssetType
/// @brief Represents different types of assets.
///
/// This enumeration defines various types of assets that can be handled by the system.
enum class AssetType
{
    None,            ///< No asset type specified.
    Mesh,            ///< A mesh asset.
    Texture,         ///< A texture asset.
    Shader,          ///< A shader asset.
    EnvironmentMap,  ///< An environment map asset.
    Script           ///< A game script.
};

/// @struct Asset
/// @brief Represents an asset with its associated data.
///
/// The Asset structure holds information about the asset, including its file path,
/// type, and specific asset data like meshes, textures, and shaders.
struct Asset
{
    String Path;       ///< File path to the asset.
    AssetType Type;    ///< Type of the asset.

    Mesh Mesh;              ///< Mesh data if the asset is a mesh.
    Texture::Ref Texture;   ///< Pointer to texture data if the asset is a texture.
    Shader Shader;          ///< Shader data if the asset is a shader.
    ScriptSource Script;    ///< Script data if the asset is a script.

    Int32 RefCount;        ///< Reference count for asset management.

    using Handle = Ref<Asset>; ///< Alias for asset pointer handle.

    /// @brief Initializes the Asset
    Asset()
        : RefCount(0), Type(AssetType::None) {
    }
};

/// @class AssetManager
/// @brief Manages asset loading, retrieval, and cleanup.
///
/// The AssetManager handles the initialization, storage, and retrieval of assets.
class AssetManager
{
public:
    /// @brief Initializes the asset manager with an RHI pointer.
    /// @param rhi The RHI pointer used for asset management.
    static void Init(RHI::Ref rhi);

    /// @brief Cleans up and releases resources held by the asset manager.
    static void Clean();

    /// @brief Retrieves an asset based on its path and type.
    /// @param path The file path of the asset.
    /// @param type The type of asset being requested.
    /// @return A handle to the retrieved asset.
    static Asset::Handle Get(const String& path, AssetType type);

    /// @brief Frees a previously loaded asset.
    /// @param handle The handle to the asset to be freed.
    static void Free(Asset::Handle handle);

private:
    /// @struct Data
    /// @brief Internal data structure for asset management.
    static struct Data
    {
        RHI::Ref mRHI; ///< Pointer to the rendering hardware interface.
        UnorderedMap<String, Asset::Handle> mAssets; ///< Storage for assets mapped by file path.
    } sData;
};
