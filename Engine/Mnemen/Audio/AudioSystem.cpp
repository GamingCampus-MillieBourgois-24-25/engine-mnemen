//
// > Notice: Amélie Heinrich @ 2025
// > Create Time: 2025-02-04 12:03:29
//

#include "AudioSystem.hpp"
#include <iostream>

#include <Core/Logger.hpp>
#include <Core/Profiler.hpp>

AudioSystem::Data AudioSystem::sData;

void AudioSystem::Init()
{
    ma_result result = ma_engine_init(nullptr, &sData.Engine);
    if (result != MA_SUCCESS) {
        LOG_CRITICAL("Failed to initialize audio engine!");
    }

    LOG_INFO("Initialized Audio system");
}

void AudioSystem::Exit()
{
    ma_engine_uninit(&sData.Engine);
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
