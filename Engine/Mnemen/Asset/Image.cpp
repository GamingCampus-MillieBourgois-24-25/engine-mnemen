//
// > Notice: Amélie Heinrich @ 2025
// > Create Time: 2025-12-04 02:29:59
//

#include <Asset/Image.hpp>
#include <Core/Logger.hpp>

#include <stb/stb_image.h>

void Image::Load(const String& path)
{
    int channels = 0;
    stbi_uc* buffer = stbi_load(path.c_str(), &Width, &Height, &channels, STBI_rgb_alpha);
    if (!buffer) {
        LOG_ERROR("Failed to load bitmap {0}", path);
        return;
    }

    Pixels.resize(Width * Height * 4);
    Levels = 1;
    memcpy(Pixels.data(), buffer, Pixels.size());
    delete buffer;
}

void Image::LoadHDR(const String& path)
{
    int channels = 0;
    stbi_us* buffer = stbi_load_16(path.c_str(), &Width, &Height, &channels, STBI_rgb_alpha);
    if (!buffer) {
        LOG_ERROR("Failed to load bitmap {0}", path);
        return;
    }

    Pixels.resize(Width * Height * 4 * sizeof(short));
    Levels = 1;
    memcpy(Pixels.data(), buffer, Pixels.size());
    delete buffer;
}
