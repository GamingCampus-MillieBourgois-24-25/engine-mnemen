//
// > Notice: Amélie Heinrich @ 2025
// > Create Time: 2025-02-04 12:03:29
//

#include "AudioSystem.hpp"
#include <iostream>

#include <Core/Logger.hpp>
#include <Core/Profiler.hpp>

ma_result AudioSystem::result;
ma_sound AudioSystem::sound;
ma_engine AudioSystem::engine;
float AudioSystem::volume = 1.0f;

void AudioSystem::Init(const char *pathAudio)
{
    LOG_INFO("Initialized Audio system");

    // Initialisation du moteur audio
    result = ma_engine_init(NULL, &engine);
    if (result != MA_SUCCESS) {
        LOG_ERROR("Failed to initialize audio engine");
        return;
    }

    // Initialisation du son
    result = ma_sound_init_from_file(&engine, pathAudio, 0, NULL, NULL, &sound);
    if (result != MA_SUCCESS) {
        LOG_ERROR("Failed to load audio file");
        return;
    }

    // --- Configuration du Listener (le moteur en gère un par défaut) ---
    ma_engine_listener_set_position(&engine, 0, 0.0f, 0.0f, 0.0f);
    ma_engine_listener_set_direction(&engine, 0, 0.0f, 0.0f, -1.0f);
    
    // --- Configuration du Son ---
    ma_sound_set_spatialization_enabled(&sound, true);
    ma_sound_set_pinned_listener_index(&sound, 0); // Attache le son au premier listener
    ma_sound_set_position(&sound, 50.0f, 50.0f, 0.0f); // Place le son à droite
}


void AudioSystem::Exit()
{
    ma_sound_uninit(&sound);
    ma_engine_uninit(&engine);
}

void AudioSystem::SetListenerPosition(float x, float y, float z)
{
    ma_engine_listener_set_position(&engine, 0, x, y, z);
}


void AudioSystem::SetVolume(float newVolume)
{
    volume = newVolume;
    ma_sound_set_volume(&sound, volume);
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
    ma_sound_start(&sound);
}
