//
// > Notice: Amélie Heinrich @ 2024
// > Create Time: 2024-12-04 20:57:18
//

#pragma once

#include <Asset/Shader.hpp>

#include <RHI/RHI.hpp>

enum class AssetType
{
    None,
    GLTF,
    Texture,
    Shader,
    EnvironmentMap
};

struct Asset
{
    String Path;
    AssetType Type;

    // GLTF Model;
    // Texture::Ref Texture;
    Shader Shader;

    UInt32 RefCount;

    using Handle = Ref<Asset>;
};

class AssetManager
{
public:
    static void Init(RHI::Ref rhi);
    static void Clean();

    static Asset::Handle Get(const String& path, AssetType type);
    static void Free(Asset::Handle handle);
private:
    static struct Data
    {
        RHI::Ref mRHI;
        UnorderedMap<String, Asset::Handle> mAssets;
    } sData;
};
