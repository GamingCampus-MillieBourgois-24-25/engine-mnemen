//
// > Notice: Amélie Heinrich @ 2025
// > Create Time: 2025-02-13 20:47:56
//

#include "Entity.hpp"

#include <Audio/AudioSystem.hpp>

void AudioSourceComponent::Init(const String& path)
{
    auto engine = AudioSystem::GetEngine();

    Free();
    Handle = AssetManager::Get(path, AssetType::Audio);
    if (Handle) {
        ma_result result = ma_sound_init_from_data_source(engine, Handle->Audio->GetDecoder(), 0, nullptr, &Sound);
        if (result != MA_SUCCESS) {
            LOG_CRITICAL("Failed to create sound source copy!");
        }
        ma_sound_set_position(&Sound, 0.0f, 0.0f, 0.0f);
    }
}

void AudioSourceComponent::Free()
{
    Stop();
    if (Handle) {
        ma_sound_uninit(&Sound);
        AssetManager::GiveBack(Handle->Path);
    }
}

void AudioSourceComponent::Play()
{
    if (Handle) {
        ma_sound_start(&Sound);
    }
}

void AudioSourceComponent::Stop()
{
    if (Handle) {
        ma_sound_stop(&Sound);
        ma_sound_seek_to_pcm_frame(&Sound, 0);
    }
}

void AudioSourceComponent::Update()
{
    if (Handle) {
        ma_sound_set_looping(&Sound, Looping);
        ma_sound_set_volume(&Sound, Volume);
    }
}
