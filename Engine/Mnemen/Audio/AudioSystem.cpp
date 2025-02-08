//
// > Notice: Amélie Heinrich @ 2025
// > Create Time: 2025-02-04 12:03:29
//

#include "AudioSystem.hpp"
#include <iostream>

#include <Core/Logger.hpp>
#include <Core/Profiler.hpp>

ma_result AudioSystem::result;
ma_decoder AudioSystem::decoder;
ma_device_config AudioSystem::deviceConfig;
ma_device AudioSystem::device;
float AudioSystem::volume = 1.0f;

void AudioSystem::Init(const char *pathAudio)
{
    LOG_INFO("Initialized Audio system");
    result = ma_decoder_init_file(pathAudio, NULL, &decoder);
    if (result != MA_SUCCESS) {
        LOG_ERROR("Failed to load audio file");
    }

    deviceConfig = ma_device_config_init(ma_device_type_playback);
    deviceConfig.playback.format = decoder.outputFormat;
    deviceConfig.playback.channels = decoder.outputChannels;
    deviceConfig.dataCallback = data_callback;
    deviceConfig.pUserData = &decoder;

    result = ma_device_init(NULL, &deviceConfig, &device);
    if (result != MA_SUCCESS) {
        LOG_ERROR("Failed to initialize device audio");
    }
}

void AudioSystem::Exit()
{
    ma_device_uninit(&device);
    ma_decoder_uninit(&decoder);
}

void AudioSystem::SetVolume(float newVolume)
{
    volume = newVolume;
}

void AudioSystem::data_callback(ma_device* pDevice, void* pOutput, const void* pInput, ma_uint32 frameCount)
{
    ma_decoder* pDecoder = (ma_decoder*)pDevice->pUserData;
    if (pDecoder == NULL) {
        return;
    }
    ma_decoder_read_pcm_frames(pDecoder, pOutput, frameCount, NULL);

    float volumeFactor = ma_volume_db_to_linear(volume);

    if (pDevice->playback.format == ma_format_f32) {
        float* samples = (float*)pOutput;
        for (ma_uint32 i = 0; i < frameCount * pDevice->playback.channels; i++) {
            samples[i] *= volumeFactor;
        }
    }

}

void AudioSystem::Update(Ref<Scene> scene)
{
    PROFILE_FUNCTION();
    ma_device_start(&device);
}
