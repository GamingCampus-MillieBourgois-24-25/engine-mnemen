//
// > Notice: Amélie Heinrich @ 2025
// > Create Time: 2025-02-04 12:03:29
//

#include "AudioSystem.hpp"

#include <Core/Logger.hpp>
#include <Core/Profiler.hpp>

void AudioSystem::Init(const char *pathAudio)
{
    ma_result result;
    ma_decoder decoder;

    LOG_INFO("Initialized Audio system");
    result = ma_decoder_init_file(pathAudio, NULL, &decoder);
    if (result != MA_SUCCESS) {
        LOG_ERROR("Failed to load audio file");
    }
}

void AudioSystem::Exit()
{

}

void AudioSystem::data_callback(ma_device* pDevice, void* pOutput, const void* pInput, ma_uint32 frameCount)
{
    ma_decoder* pDecoder = (ma_decoder*)pDevice->pUserData;
    if (pDecoder == NULL) {
        return;
    }
    ma_decoder_read_pcm_frames(pDecoder, pOutput, frameCount, NULL);
}

void AudioSystem::Update(Ref<Scene> scene)
{
    PROFILE_FUNCTION();
}
