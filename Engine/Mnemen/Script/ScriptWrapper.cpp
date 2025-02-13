//
// > Notice: Amélie Heinrich @ 2025
// > Create Time: 2025-02-13 18:41:00
//

#include "ScriptWrapper.hpp"

#include <Core/Application.hpp>
#include <World/Scene.hpp>
#include <World/Entity.hpp>

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

TransformComponent& LuaWrapper::LuaEntity::GetTransform(int entity)
{
    auto scene = Application::Get()->GetScene();
    auto registry = scene->GetRegistry();

    Entity wrap(registry);
    wrap.ID = (entt::entity)entity;

    return wrap.GetComponent<TransformComponent>();
}
