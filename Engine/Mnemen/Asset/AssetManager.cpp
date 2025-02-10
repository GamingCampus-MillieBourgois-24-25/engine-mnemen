//
// > Notice: Amélie Heinrich @ 2025
// > Create Time: 2025-12-04 21:02:03
//

#include <Asset/AssetManager.hpp>
#include <Asset/AssetCacher.hpp>

#include <Core/Logger.hpp>
#include <RHI/Uploader.hpp>

AssetManager::Data AssetManager::sData;

void AssetManager::Init(RHI::Ref rhi)
{
    sData.mRHI = rhi;

    LOG_INFO("Initialized Asset Manager");
}

void AssetManager::Clean()
{
    sData.mAssets.clear();
}

Asset::Handle AssetManager::Get(const String& path, AssetType type)
{
    if (sData.mAssets.count(path) > 0) {
        sData.mAssets[path]->RefCount++;
        return sData.mAssets[path];
    }

    Asset::Handle asset = MakeRef<Asset>();
    asset->RefCount = 1;
    asset->Type = type;
    asset->Path = path;

    switch (type) {
        case AssetType::Mesh: {
            LOG_DEBUG("Loading Mesh {0}", path);
            asset->Mesh.Load(sData.mRHI, path);
            break;
        }
        case AssetType::Texture: {
            LOG_DEBUG("Loading texture {0}", path);
        
            if (AssetCacher::IsCached(path) && true) {
                AssetFile file = AssetCacher::ReadAsset(path);
                
                TextureDesc desc;
                desc.Width = file.Header.TextureHeader.Width;
                desc.Height = file.Header.TextureHeader.Height;
                desc.Levels = file.Header.TextureHeader.Levels;
                desc.Depth = 1;
                desc.Name = path;
                desc.Format = TextureFormat::BC7;
                desc.Usage = TextureUsage::ShaderResource;
                asset->Texture = sData.mRHI->CreateTexture(desc);
                asset->Texture->Tag(ResourceTag::ModelTexture);
        
                Uploader::EnqueueTextureUpload(file.Bytes, asset->Texture);
            } else {
                Image image;
                image.Load(path);
        
                TextureDesc desc;
                desc.Width = image.Width;
                desc.Height = image.Height;
                desc.Levels = image.Levels;
                desc.Depth = 1;
                desc.Name = path;
                desc.Format = TextureFormat::RGBA8;
                desc.Usage = TextureUsage::ShaderResource;
                asset->Texture = sData.mRHI->CreateTexture(desc);
                asset->Texture->Tag(ResourceTag::ModelTexture);
            
                Uploader::EnqueueTextureUpload(image, asset->Texture);
            }
            break;
        }
        case AssetType::Shader: {
            LOG_INFO("Loading shader {0}", path);

            if (AssetCacher::IsCached(path) && true) {
                AssetFile file = AssetCacher::ReadAsset(path);
                asset->Shader.Type = file.Header.ShaderHeader.Type;
                asset->Shader.Bytecode.resize(file.Bytes.size());
                memcpy(asset->Shader.Bytecode.data(), file.Bytes.data(), file.Bytes.size());
            } else {
                ShaderType type = AssetCacher::GetShaderTypeFromPath(path);
                asset->Shader = ShaderCompiler::Compile(path, AssetCacher::GetEntryPointFromShaderType(type), type);
            }
            break;
        }
        case AssetType::Script: {
            LOG_INFO("Loading script {0}", path);
            asset->Script.Load(path);
            break;
        }
    }

    sData.mAssets[path] = asset;
    return asset;
}

void AssetManager::Free(Asset::Handle handle)
{
    sData.mAssets[handle->Path]->RefCount--;
    if (sData.mAssets[handle->Path]->RefCount == 0) {
        LOG_DEBUG("Freeing asset {0}", handle->Path);
        sData.mAssets[handle->Path].reset();
        sData.mAssets.erase(handle->Path);
    }
}
