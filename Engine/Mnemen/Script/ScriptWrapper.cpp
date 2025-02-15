//
// > Notice: Amélie Heinrich @ 2025
// > Create Time: 2025-02-13 18:41:00
//

#include "ScriptWrapper.hpp"

#include <Core/Application.hpp>
#include <World/Scene.hpp>
#include <World/Entity.hpp>

void LuaWrapper::LuaEntity::DeleteEntity(int entity)
{
    auto scene = Application::Get()->GetScene();
    auto registry = scene->GetRegistry();

    Entity wrap(registry);
    wrap.ID = (entt::entity)entity;

    scene->RemoveEntity(wrap);
}

String LuaWrapper::LuaEntity::GetName(int entity)
{
    auto scene = Application::Get()->GetScene();
    auto registry = scene->GetRegistry();

    Entity wrap(registry);
    wrap.ID = (entt::entity)entity;

    return wrap.GetComponent<TagComponent>().Tag;
}

void LuaWrapper::LuaEntity::SetName(int entity, const char* name)
{
    auto scene = Application::Get()->GetScene();
    auto registry = scene->GetRegistry();

    Entity wrap(registry);
    wrap.ID = (entt::entity)entity;

    wrap.GetComponent<TagComponent>().Tag = name;
}

int LuaWrapper::LuaEntity::GetEntityByName(const char* name)
{
    auto scene = Application::Get()->GetScene();
    auto registry = scene->GetRegistry();

    auto view = registry->view<TagComponent>();
    for (auto [id, tag] : view.each()) {
        if (tag.Tag.compare(name) == 0) {
            return (int)id;
        }
    }
    return -1;
}

TransformComponent& LuaWrapper::LuaEntity::GetTransform(int entity)
{
    auto scene = Application::Get()->GetScene();
    auto registry = scene->GetRegistry();

    Entity wrap(registry);
    wrap.ID = (entt::entity)entity;

    return wrap.GetComponent<TransformComponent>();
}

CameraComponent& LuaWrapper::LuaEntity::GetCamera(int entity)
{
    auto scene = Application::Get()->GetScene();
    auto registry = scene->GetRegistry();

    Entity wrap(registry);
    wrap.ID = (entt::entity)entity;

    return wrap.GetComponent<CameraComponent>();
}

AudioSourceComponent& LuaWrapper::LuaEntity::GetAudioSource(int entity)
{
    auto scene = Application::Get()->GetScene();
    auto registry = scene->GetRegistry();

    Entity wrap(registry);
    wrap.ID = (entt::entity)entity;

    return wrap.GetComponent<AudioSourceComponent>();
}
