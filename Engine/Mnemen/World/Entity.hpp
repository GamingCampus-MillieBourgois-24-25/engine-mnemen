//
// > Notice: Amélie Heinrich @ 2025
// > Create Time: 2025-02-03 22:42:24
//

#pragma once

#include <entt/entt.hpp>
#include <Core/Common.hpp>
#include <Core/Logger.hpp>

#include <Asset/AssetManager.hpp>
#include <Script/ScriptInstance.hpp>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>

#include <Utility/UUID.hpp>

class Scene;

/// @brief An entity in the world
struct Entity
{
    /// @brief The internal entt ID
    entt::entity ID = entt::null;
    /// @brief The parent entt registry
    entt::registry* ParentRegistry;

    /// @brief Checks if the entity is valid (not null)
    operator bool() const {
        return ID != entt::null;
    }

    /// @brief Implicit conversion to `entt::entity`
    operator entt::entity() const {
        return ID;
    }

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
        if (HasComponent<T>()) {
            LOG_WARN("da fuck");
        }
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

    /// @brief Attaches a parent to the current entity
    /// @param parent The new parent of the entity
    void SetParent(Entity parent);

    /// @brief Removes this entity from its parent
    void RemoveParent();

    /// @brief Get the entity's current parent
    /// @return The current parent of the entity
    Entity GetParent();

    /// @brief Returns whether or not the entity has a parent
    /// @return True if it has one, otherwise false
    bool HasParent();

    /// @brief Returns a list of child entities
    Vector<Entity> GetChildren();

    /// @brief Returns the world transform of the entity
    /// @return The world transform of the entity
    glm::mat4 GetWorldTransform();

    /// @brief Returns the local transform of the entity
    /// @return The local transform of the entity
    glm::mat4 GetLocalTransform();

    /// @brief Sets the local transform of the entity
    /// @param localTransform local transform of the entity
    void SetLocalTransform(glm::mat4 localTransform);
};

/// @brief A component giving a name to an entity
struct TagComponent
{
    /// @brief The name of the entity
    String Tag = "";
};

/// @brief A component making the entity private
struct PrivateComponent
{
    /// @brief Placeholder. If the entity has this component, it's private anyway.
    int Placeholder;
};

/// @brief A component representing the children state of an entity
struct ParentComponent
{
    /// @brief The parent of the current entity
    Entity Parent;
};

/// @brief a component representing the parent state of an entity
struct ChildrenComponent
{
    /// @brief The children of the current entity
    Vector<Entity> Children;
};

/// @brief A component representing a transform -- the spatial representation of the object.
struct TransformComponent
{
    /// @brief The position of the object
    glm::vec3 Position = glm::vec3(0.0f);
    /// @brief The scale of the object
    glm::vec3 Scale = glm::vec3(1.0f);
    /// @brief The rotation of the object
    glm::quat Rotation = glm::quat();
    /// @brief Local Matrix
    glm::mat4 Matrix = glm::mat4(1.0f);

    /// @brief Updates the transform matrix
    void Update();
};

/// @brief A component holding a mesh
struct MeshComponent
{
    /// @brief Used for debugging
    Entity ParentEntity;

    /// @brief The asset handle pointing to the mesh
    Asset::Handle MeshAsset;

    /// @brief Whether or not the model is loaded
    bool Loaded = false;

    /// @brief Initializes the component and loads the mesh at the given string
    /// @param string The path of the component
    void Init(const String& string);

    /// @brief Manually free the mesh asset
    void Free();
};

/// @brief A component holding camera information
struct CameraComponent
{
    /// @brief This is a number so that editor camera can have a priority of 2!
    int Primary = 1;
    /// @brief The field of view of the camera, in degrees
    float FOV = 90.0f;
    /// @brief The near plane of the camera
    float Near = 0.1f;
    /// @brief The far plane of the camera
    float Far = 200.0f;
    /// @brief The aspect ratio of the camera, set to 16:9 by default
    float AspectRatio = 1.77777777778f;

    /// @brief The view matrix of the camera
    glm::mat4 View = glm::mat4(1.0f);
    /// @brief The projection matrix of the camera
    glm::mat4 Projection = glm::mat4(1.0f);

    /// @brief Updates the camera matrices using the transform's position and rotation
    /// @param Position The position pulled from the transform
    /// @param Rotation The rotation pulled from the transform
    void Update(glm::vec3 Position, glm::quat Rotation);
};

/// @brief A component holding a game script
struct ScriptComponent
{
    struct EntityScript
    {
        Util::UUID ID;
        Asset::Handle Handle;
        Ref<ScriptInstance> Instance;

        EntityScript();
        ~EntityScript();

        void Load(const String& path);
    };

    Vector<Ref<EntityScript>> Instances;

    void AddEmptyScript();
    void PushScript(const String& path);
};
