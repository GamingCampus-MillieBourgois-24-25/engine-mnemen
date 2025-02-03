//
// > Notice: Amélie Heinrich @ 2024
// > Create Time: 2024-12-04 21:28:06
//

#pragma once

#include <Asset/AssetManager.hpp>
#include <Asset/Shader.hpp>
#include <Core/File.hpp>

struct AssetFile
{
    struct Header
    {
        File::Filetime Filetime;
        AssetType Type;

        struct {
            int Width;
            int Height;
            int Levels;
        } TextureHeader;

        struct {
            ShaderType Type;
        } ShaderHeader;
    } Header;
    Vector<UInt8> Bytes;
};

class AssetCacher
{
public:
    static void Init(const String& assetDirectory);
    static void CacheAsset(const String& normalPath);
    static bool IsCached(const String& normalPath);

    static AssetFile ReadAsset(const String& path);
private:
    friend class AssetManager;

    // static struct Data
    // {
    //     nvtt::Context mContext;
    // } sData;

    static AssetFile ReadAssetHeader(const String& path);
    static String GetEntryPointFromShaderType(ShaderType type);
    static ShaderType GetShaderTypeFromPath(const String& path);
    static AssetType GetAssetTypeFromPath(const String& normalPath);
    static String GetCachedAsset(const String& normalPath);
};
