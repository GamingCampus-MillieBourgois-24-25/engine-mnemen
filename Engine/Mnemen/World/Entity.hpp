//
// > Notice: Amélie Heinrich @ 2025
// > Create Time: 2025-02-03 22:42:24
//

#pragma once

#include <entt/entt.hpp>
#include <Core/Common.hpp>

class Scene;

struct Entity
{
    entt::entity ID;
    String Name;
    entt::registry* ParentRegistry;

    Entity(entt::registry* registry = nullptr)
        : ParentRegistry(registry)
    {}

    /// @note(ame): methods
    template<typename T, typename... Arguments>
    T& AddComponent(Arguments&&... args)
    {
        return ParentRegistry->emplace<T>(ID, std::forward<Arguments>(args)...);
    }

    template<typename T>
    T& GetComponent()
    {
        return ParentRegistry->get<T>(ID);
    }

    template<typename T>
    void RemoveComponent()
    {
        ParentRegistry->remove<T>(ID);
    }

    template<typename T>
    bool HasComponent()
    {
        return ParentRegistry->any_of<T>(ID);
    }
};
