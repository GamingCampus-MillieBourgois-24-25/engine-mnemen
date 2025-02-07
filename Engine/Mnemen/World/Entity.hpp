//
// > Notice: Amélie Heinrich @ 2025
// > Create Time: 2025-02-03 22:42:24
//

#pragma once

#include <entt/entt.hpp>
#include <Core/Common.hpp>

class Scene;

/// @brief An entity in the world
struct Entity
{
    /// @brief The internal entt ID
    entt::entity ID;
    /// @brief The name of the entity
    String Name;
    /// @brief The parent entt registry
    entt::registry* ParentRegistry;
    /// @brief If this is set to true, it will not be serialized or shown in the editor
    bool Private = false;

    /// @brief Constructor -- sets the parent registry as well
    /// @param registry The parent registry the entity was created from
    Entity(entt::registry* registry = nullptr)
        : ParentRegistry(registry)
    {}

    /// @brief Adds a component to the entity container
    /// @tparam T The type of the component
    /// @tparam ...Arguments The type of the arguments
    /// @param ...args The value of the arguments
    /// @return A reference to the added component
    template<typename T, typename... Arguments>
    T& AddComponent(Arguments&&... args)
    {
        return ParentRegistry->emplace<T>(ID, std::forward<Arguments>(args)...);
    }

    /// @brief Gets the component (if it has one)
    /// @tparam T The type of the component
    /// @return A reference to the component
    template<typename T>
    T& GetComponent()
    {
        return ParentRegistry->get<T>(ID);
    }

    /// @brief Removes the given component if it has one
    /// @tparam T The type of the component
    template<typename T>
    void RemoveComponent()
    {
        ParentRegistry->remove<T>(ID);
    }

    /// @brief Returns whether or not an entity has a component
    /// @tparam T The type of the component
    /// @return True if it has the component, otherwise false
    template<typename T>
    bool HasComponent()
    {
        return ParentRegistry->any_of<T>(ID);
    }
};
