//
// > Notice: Amélie Heinrich @ 2025
// > Create Time: 2025-12-04 21:39:09
//

#include <Asset/AssetCacher.hpp>
#include <Core/Logger.hpp>
#include <Core/Application.hpp>

#include <filesystem>

AssetCacher::Data AssetCacher::sData;

/// @brief A custom error handler for NVTT (NVIDIA Texture Tools).
/// @details This class is used to handle different error scenarios that may arise during texture compression using NVTT.
class NVTTErrorHandler : public nvtt::ErrorHandler
{
public:
    /// @brief Handles errors encountered during texture compression.
    /// @param e The error encountered by NVTT.
    virtual void error(nvtt::Error e) override {
       switch (e) {
           case nvtt::Error::Error_UnsupportedOutputFormat: {
               LOG_ERROR("[nvtt] Error_UnsupportedOutputFormat");
               break;
           }
           case nvtt::Error::Error_UnsupportedFeature: {
               LOG_ERROR("[nvtt] Error_UnsupportedFeature");
               break;
           }
           case nvtt::Error::Error_Unknown: {
               LOG_ERROR("[nvtt] Error_Unknown");
               break;
           }
           case nvtt::Error::Error_InvalidInput: {
               LOG_ERROR("[nvtt] Error_InvalidInput");
               break;
           }
           case nvtt::Error::Error_FileWrite: {
               LOG_ERROR("[nvtt] Error_FileWrite");
               break;
           }
           case nvtt::Error::Error_FileOpen: {
               LOG_ERROR("[nvtt] Error_FileOpen");
               break;
           }
           case nvtt::Error::Error_CudaError: {
               LOG_ERROR("[nvtt] Error_CudaError");
               break;
           }
           default: {
               LOG_ERROR("[nvtt] unknown error!");
               break;
           }
       }
    }
};

/// @brief A custom output handler for NVTT to write texture data to memory.
/// @details This class is used to handle the output of the compressed texture and store it into a `Vector<UInt8>`.
class TextureWriter : public nvtt::OutputHandler
{
public:
    /// @brief Constructor that initializes the output handler with a vector of bytes to store texture data.
    /// @param bytes A pointer to a `Vector<UInt8>` where texture data will be written.
    TextureWriter(Vector<UInt8>* bytes) {
        mBytes = bytes;
    }

    /// @brief Destructor that resets the pointer to the vector.
    ~TextureWriter() {
        mBytes = nullptr;
    }

    /// @brief Called when a new image begins during texture compression.
    /// @param size The size of the image data.
    /// @param width The width of the image.
    /// @param height The height of the image.
    /// @param depth The depth of the image.
    /// @param face The face index for cube maps.
    /// @param miplevel The mip level of the image.
    virtual void beginImage(int size, int width, int height, int depth, int face, int miplevel) override {}

    /// @brief Called when the current image ends during texture compression.
    virtual void endImage() override {}

    /// @brief Writes data to the `Vector<UInt8>`.
    /// @param data A pointer to the data to be written.
    /// @param size The size of the data to write.
    /// @return `true` if data was written successfully, `false` otherwise.
    virtual bool writeData(const void* data, int size) override {
        UInt8* readableData = (UInt8*)data;
        for (int i = 0; i < size; i++) {
            mBytes->push_back(readableData[i]);
        }
        return true;
    }

private:
    Vector<UInt8>* mBytes; ///< Pointer to a `Vector<UInt8>` where texture data is written.
};


String AssetCacher::GetCachedAsset(const String& normalPath)
{
    const UInt64 m = 0xc6a4a7935bd1e995ULL;
    const UInt32 r = 47;

    UInt64 h = 1000 ^ (normalPath.size() * m);
    const UInt64 * data = (const UInt64 *)normalPath.data();
    const UInt64 * end = data + (normalPath.size() / 8);
    while (data != end) {
        UInt64 k = *data++;
        k *= m;
        k ^= k >> r;
        k *= m;
        
        h ^= k;
        h *= m;
    }

    const UInt8 * data2 = (const UInt8*)data;
    switch(normalPath.size() & 7) {
        case 7: h ^= UInt64(data2[6]) << 48;
        case 6: h ^= UInt64(data2[5]) << 40;
        case 5: h ^= UInt64(data2[4]) << 32;
        case 4: h ^= UInt64(data2[3]) << 24;
        case 3: h ^= UInt64(data2[2]) << 16;
        case 2: h ^= UInt64(data2[1]) << 8;
        case 1: h ^= UInt64(data2[0]);
                h *= m;
    };
    
    h ^= h >> r;
    h *= m;
    h ^= h >> r;
    return ".cache/" + String(std::to_string(h)) + ".ma";
}

AssetFile AssetCacher::ReadAsset(const String& path)
{
    String cached = GetCachedAsset(path);
    if (!File::Exists(cached)) {
        CacheAsset(path);
    }
    
    AssetFile result = {};

    UInt8* Bytes = (UInt8*)File::ReadBytes(cached);
    memcpy(&result.Header, Bytes, sizeof(AssetFile::Header));

    UInt64 size = File::GetFileSize(cached);
    result.Bytes.resize(size - sizeof(AssetFile::Header));
    memcpy(result.Bytes.data(), Bytes + sizeof(AssetFile::Header), result.Bytes.size());

    delete[] Bytes;
    return result;
}

AssetFile AssetCacher::ReadAssetHeader(const String& path)
{
    String cached = GetCachedAsset(path);

    AssetFile result = {};
    File::ReadBytes(cached, &result.Header, sizeof(result.Header));
    return result;
}

String AssetCacher::GetEntryPointFromShaderType(ShaderType type)
{
    switch (type) {
        case ShaderType::Vertex: {
            return "VSMain";
        }
        case ShaderType::Fragment: {
            return "PSMain";
        }
        case ShaderType::Compute: {
            return "CSMain";
        }
        case ShaderType::Hull: {
            return "HSMain";
        }
        case ShaderType::Domain: {
            return "DSMain";
        }
        case ShaderType::Mesh: {
            return "MSMain";
        }
        case ShaderType::Amplification: {
            return "ASMain";
        }
        case ShaderType::Library: {
            return "";
        }
    }
    return "";
}

ShaderType AssetCacher::GetShaderTypeFromPath(const String& path)
{
    if (path.find("Vertex") != String::npos) {
        return ShaderType::Vertex;
    }
    if (path.find("Fragment") != String::npos) {
        return ShaderType::Fragment;
    }
    if (path.find("Compute") != String::npos) {
        return ShaderType::Compute;
    }
    if (path.find("Hull") != String::npos) {
        return ShaderType::Hull;
    }
    if (path.find("Domain") != String::npos) {
        return ShaderType::Domain;
    }
    if (path.find("Mesh") != String::npos) {
        return ShaderType::Mesh;
    }
    if (path.find("Amplification") != String::npos) {
        return ShaderType::Amplification;
    }
    if (path.find("Lib") != String::npos) {
        return ShaderType::Library;
    }
    return ShaderType::None;
}

AssetType AssetCacher::GetAssetTypeFromPath(const String& normalPath)
{
    String extension = File::GetFileExtension(normalPath);

    if (extension == ".png" || extension == ".jpg" || extension == ".jpeg")
        return AssetType::Texture;
    if (extension == ".hlsl")
        return AssetType::Shader;

    return AssetType::None;
}

void AssetCacher::CacheAsset(const String& normalPath)
{
    CompressionFormat format = Application::Get()->GetProject()->Settings.Format;

    AssetType type = GetAssetTypeFromPath(normalPath);
    if (type == AssetType::None) {
        return;
    }

    File::Filetime assetFiletime = File::GetLastModified(normalPath);
    String cached = GetCachedAsset(normalPath);
    if (File::Exists(cached)) {
        // Read only the header
        AssetFile cachedFile = ReadAssetHeader(normalPath);
        if (assetFiletime == cachedFile.Header.Filetime) {
            return;
        }
    }

    AssetFile file;
    file.Header.Filetime = assetFiletime;

    switch (type) {
        case AssetType::Texture: {
            nvtt::Surface image;
            if (!image.load(normalPath.c_str())) {
                LOG_ERROR("Failed to load texture {0}", normalPath);
                return;
            }

            int imageWidth = image.width();
            int imageHeight = image.height();
            if (imageWidth != imageHeight) {
                LOG_WARN("Image {0} cannot be compressed due to dimensions that are not squares of 2.", normalPath);
                return;
            }
            int mipCount = image.countMipmaps();
            int finalMipCount = glm::max(1, mipCount - 2); // (Remove mip 2x2 and 1x1)

            file.Header.TextureHeader.Width = imageWidth;
            file.Header.TextureHeader.Height = imageHeight;
            file.Header.TextureHeader.Levels = finalMipCount;
            LOG_INFO("Caching texture {0} ({1}, {2}, {3})", normalPath, imageWidth, imageHeight, finalMipCount);

            TextureWriter writer(&file.Bytes);
            NVTTErrorHandler errorHandler;

            nvtt::OutputOptions outputOptions;
            outputOptions.setErrorHandler(reinterpret_cast<nvtt::ErrorHandler*>(&errorHandler));
            outputOptions.setOutputHandler(reinterpret_cast<nvtt::OutputHandler*>(&writer));

            nvtt::CompressionOptions compressionOptions;
            compressionOptions.setFormat(format == CompressionFormat::BC7 ? nvtt::Format::Format_BC7 : nvtt::Format::Format_BC3);

            for (int i = 0; i < finalMipCount; i++) {
                if (!sData.mContext.compress(image, 0, i, compressionOptions, outputOptions)) {
                    LOG_ERROR("Failed to compress texture!");
                }

                // Prepare the next mip:
                image.toLinearFromSrgb();
                image.premultiplyAlpha();

                image.buildNextMipmap(nvtt::MipmapFilter_Box);

                image.demultiplyAlpha();
                image.toSrgb();
            }
            break;
        }
        case AssetType::Shader: {
            ShaderType type = GetShaderTypeFromPath(normalPath);
            file.Header.ShaderHeader.Type = type;
            if (type == ShaderType::None)
                return;
            
            LOG_INFO("Caching shader {0}", normalPath);
            Shader shader = ShaderCompiler::Compile(normalPath, GetEntryPointFromShaderType(type), type);
            file.Bytes.resize(shader.Bytecode.size());
            memcpy(file.Bytes.data(), shader.Bytecode.data(), shader.Bytecode.size());
            break;
        }
    }

    Vector<UInt8> bytesToWrite;
    bytesToWrite.resize(sizeof(AssetFile::Header));
    memcpy(bytesToWrite.data(), &file.Header, sizeof(AssetFile::Header));
    bytesToWrite.insert(bytesToWrite.end(), file.Bytes.begin(), file.Bytes.end());

    File::WriteBytes(cached, bytesToWrite.data(), bytesToWrite.size());
}

bool AssetCacher::IsCached(const String& normalPath)
{
    if (File::Exists(GetCachedAsset(normalPath)))
        return true;
    return false;
}

void AssetCacher::Init(const String& assetDirectory)
{
    if (!File::Exists(".cache")) {
        File::CreateDirectoryFromPath(".cache");
    }

    sData.mContext.enableCudaAcceleration(true);
    if (!sData.mContext.isCudaAccelerationEnabled()) {
        LOG_WARN("No CUDA compression for you, good luck!");
    }

    for (const auto& dirEntry : std::filesystem::recursive_directory_iterator(assetDirectory)) {
        String entryPath = dirEntry.path().string();
        std::replace(entryPath.begin(), entryPath.end(), '\\', '/');
    
        CacheAsset(entryPath);
    }

    LOG_INFO("Initialized Asset Cacher");
}
