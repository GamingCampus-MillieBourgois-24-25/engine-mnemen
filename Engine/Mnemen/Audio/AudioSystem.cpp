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
ma_delay_node AudioSystem::delayNode;
float AudioSystem::volume = 1.0f;

// Init delayNode
void AudioSystem::InitDelayNode() {

    ma_delay_node_config delayNodeConfig;
    ma_uint32 channels;
    ma_uint32 sampleRate;

    channels   = ma_engine_get_channels(&engine);
    sampleRate = ma_engine_get_sample_rate(&engine);

    delayNodeConfig = ma_delay_node_config_init(channels, sampleRate, (ma_uint32)(sampleRate * 0.2f), 0.80f);

    result = ma_delay_node_init(ma_engine_get_node_graph(&engine), &delayNodeConfig, NULL, &delayNode);
    if (result != MA_SUCCESS) {
        LOG_ERROR("Failed to initialize delay node.");
        return;
    }

    /* Connect the output of the delay node to the input of the endpoint. */
    ma_node_attach_output_bus(&delayNode, 0, ma_engine_get_endpoint(&engine), 0);

}

// Init audio
void AudioSystem::Init(const char *pathAudio)
{
    LOG_INFO("Initialized Audio system");

    // Initialisation du moteur audio
    result = ma_engine_init(NULL, &engine);
    if (result != MA_SUCCESS) {
        LOG_ERROR("Failed to initialize audio engine");
        return;
    }

    InitDelayNode();

    // Initialisation du son
    result = ma_sound_init_from_file(&engine, pathAudio, 0, NULL, NULL, &sound);
    if (result != MA_SUCCESS) {
        LOG_ERROR("Failed to load audio file");
        return;
    }

    ma_node_attach_output_bus(&sound, 0, &delayNode, 0);

    // --- Configuration du Listener ---
    ma_engine_listener_set_position(&engine, 0, 0.0f, 0.0f, 0.0f);
    ma_engine_listener_set_direction(&engine, 0, 0.0f, 0.0f, -1.0f);
    
    // --- Configuration du Son ---
    ma_sound_set_spatialization_enabled(&sound, true);
    ma_sound_set_pinned_listener_index(&sound, 0); // Attache le son au premier listener
    ma_sound_set_position(&sound, 0.0f, 0.0f, 0.0f);
}

// Free audio
void AudioSystem::Exit()
{
    ma_sound_uninit(&sound);
    ma_delay_node_uninit(&delayNode, NULL);
    ma_engine_uninit(&engine);
}

// Set the position of the listener
void AudioSystem::SetListenerPosition(float x, float y, float z)
{
    ma_engine_listener_set_position(&engine, 0, x, y, z);
}

// Set a new volume to the sound
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

// Update the scene
void AudioSystem::Update(Ref<Scene> scene)
{
    PROFILE_FUNCTION();
    ma_sound_start(&sound);
}
