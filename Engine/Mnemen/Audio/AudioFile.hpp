//
// > Notice: Amélie Heinrich @ 2025
// > Create Time: 2025-02-13 20:14:28
//

#pragma once

#include <Core/Common.hpp>

#include <miniaudio.h>

class AudioFile
{
public:
    using Ref = Ref<AudioFile>;

    AudioFile(const String& path);
    ~AudioFile();

    bool IsValid() { return mValid; }
    ma_decoder* GetDecoder() { return &mDecoder; }
private:
    bool mValid = false;
    ma_decoder mDecoder;
};
