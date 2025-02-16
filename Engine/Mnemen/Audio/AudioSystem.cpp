//
// > Notice: Amélie Heinrich @ 2025
// > Create Time: 2025-02-04 12:03:29
//

#include "AudioSystem.hpp"
#include <iostream>

#include <Core/Logger.hpp>
#include <Core/Profiler.hpp>

AudioSystem::Data AudioSystem::sData;

void DataCallback(ma_device *pDevice, void *output, const void *input, ma_uint32 frameCount)
{
    ma_engine* engine = (ma_engine*)pDevice->pUserData;

    if (engine->pResourceManager != nullptr) {
        if ((engine->pResourceManager->config.flags & MA_RESOURCE_MANAGER_FLAG_NO_THREADING) != 0) {
            ma_resource_manager_process_next_job(engine->pResourceManager);
        }
    }
    ma_engine_read_pcm_frames(engine, output, frameCount, nullptr);
}

void AudioSystem::Init()
{
    ma_device_config deviceConfig = ma_device_config_init(ma_device_type_playback);
    deviceConfig.playback.format = ma_format_f32;
    deviceConfig.playback.channels = 2;
    deviceConfig.sampleRate = 48000;
    deviceConfig.dataCallback = DataCallback;
    deviceConfig.pUserData = &sData.Engine;

    ma_result result = ma_device_init(nullptr, &deviceConfig, &sData.Device);
    if (result != MA_SUCCESS) {
        LOG_CRITICAL("Failed to initialize audio device!");
    }

    char name[512];
    ma_device_get_name(&sData.Device, ma_device_type_playback, name, 512, nullptr);
    LOG_INFO("Using audio device {0}", name);

    ma_engine_config engineConfig = ma_engine_config_init();
    engineConfig.pDevice = &sData.Device;
    engineConfig.listenerCount = 1;

    result = ma_engine_init(&engineConfig, &sData.Engine);
    if (result != MA_SUCCESS) {
        LOG_CRITICAL("Failed to initialize audio engine!");
    }

    LOG_INFO("Initialized Audio system");
}

void AudioSystem::Exit()
{
    ma_engine_uninit(&sData.Engine);
    ma_device_uninit(&sData.Device);
}

void AudioSystem::Awake(Ref<Scene> scene)
{
    entt::registry* registry = scene->GetRegistry();
    auto view = registry->view<AudioSourceComponent>();

    for (auto [id, source] : view.each()) {
        if (source.PlayOnAwake) {
            source.Play();
        }
    }
}

void AudioSystem::Update(Ref<Scene> scene)
{
    PROFILE_FUNCTION();

    entt::registry* registry = scene->GetRegistry();
    auto view = registry->view<AudioSourceComponent>();

    for (auto [id, source] : view.each()) {
        source.Update();
    }
}

void AudioSystem::Quit(Ref<Scene> scene)
{
    entt::registry* registry = scene->GetRegistry();
    auto view = registry->view<AudioSourceComponent>();

    for (auto [id, source] : view.each()) {
        source.Stop();
    }
}
