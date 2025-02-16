//
// > Notice: Amélie Heinrich @ 2025
// > Create Time: 2025-02-13 20:14:41
//

#include "AudioFile.hpp"

#include <Core/Logger.hpp>
#include <Audio/AudioSystem.hpp>

AudioFile::AudioFile(const String& path)
{
    auto engine = AudioSystem::GetEngine();

    ma_result result = ma_decoder_init_file(path.c_str(), nullptr, &mDecoder);
    if (result != MA_SUCCESS) {
        LOG_ERROR("Failed to load audio file {0}", path);
        mValid = false;
    }
    mValid = true;
}

AudioFile::~AudioFile()
{
    ma_decoder_uninit(&mDecoder);
    mValid = false;
}
