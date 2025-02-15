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
#include <Renderer/PostProcessVolume.hpp>

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
    /// @brief The post process volume of the camera
    PostProcessVolume Volume;

    /// @brief The view matrix of the camera
    glm::mat4 View = glm::mat4(1.0f);
    /// @brief The projection matrix of the camera
    glm::mat4 Projection = glm::mat4(1.0f);

    /// @brief Updates the camera matrices using the transform's position and rotation
    /// @param Position The position pulled from the transform
    /// @param Rotation The rotation pulled from the transform
    void Update(glm::vec3 Position, glm::quat Rotation);
};

/// @struct ScriptComponent
/// @brief A component that holds scripts for entities.
///
/// This structure is used to store and manage the scripts associated with an entity in the game.
/// It provides functionality to load and add scripts dynamically at runtime.
struct ScriptComponent
{
    /// @struct EntityScript
    /// @brief A structure that holds information about a script associated with an entity.
    ///
    /// This structure holds the unique ID, asset handle, and the instance of a script for an entity.
    struct EntityScript
    {
        Util::UUID ID; ///< The unique identifier for the script.
        Asset::Handle Handle; ///< The handle to the asset representing the script.
        Ref<ScriptInstance> Instance; ///< A reference to the script instance for execution.

        /// @brief Default constructor for the EntityScript structure.
        EntityScript();

        /// @brief Destructor for the EntityScript structure.
        ~EntityScript();

        /// @brief Loads the script from a specified file path.
        ///
        /// This function loads a script into the entity by its file path. The script will be associated
        /// with the `ID` and `Handle` and can be instantiated for execution.
        ///
        /// @param path The file path to the script to load.
        void Load(const String& path);
    };

    /// @brief A vector of script instances associated with the entity.
    Vector<Ref<EntityScript>> Instances;

    /// @brief Adds an empty script to the `Instances` vector.
    ///
    /// This function adds an empty script to the collection of script instances, allowing it to be
    /// populated or loaded later.
    void AddEmptyScript();

    /// @brief Adds a script by loading it from a specified file path.
    ///
    /// This function loads a script from the given file path and adds it to the collection of script instances.
    ///
    /// @param path The file path to the script to load and add.
    void PushScript(const String& path);
};


/// @struct AudioSourceComponent
/// @brief A component that handles audio playback for an entity.
///
/// This structure is responsible for managing and controlling audio playback for an entity,
/// including loading sounds, controlling playback, and adjusting properties like volume and looping.
struct AudioSourceComponent
{
    /// @brief The handle to the audio asset representing the sound.
    Asset::Handle Handle;

    /// @brief The sound instance that manages the audio playback.
    ma_sound Sound;

    /// @brief Flag indicating whether the sound should loop during playback.
    bool Looping = false;

    /// @brief Flag indicating whether the sound should start playing automatically when initialized.
    bool PlayOnAwake = false;

    /// @brief The volume of the sound, ranging from 0.0f (muted) to 1.0f (full volume).
    float Volume = 1.0f;

    /// @brief Initializes the audio source with a specified sound file.
    ///
    /// This function loads and prepares the sound file for playback from the given path.
    ///
    /// @param path The file path to the sound asset to load.
    void Init(const String& path);

    /// @brief Frees any resources associated with the audio source.
    ///
    /// This function releases the sound resource and resets the `AudioSourceComponent` to an idle state.
    void Free();

    /// @brief Plays the sound from the beginning.
    ///
    /// This function starts the audio playback from the beginning, respecting the `Looping`
    /// and `Volume` properties.
    void Play();

    /// @brief Stops the sound if it is currently playing.
    ///
    /// This function stops the audio playback and resets it to the idle state.
    void Stop();

    /// @brief Updates the audio source, typically called every frame.
    ///
    /// This function can be used to update properties or check the status of the audio playback
    /// during the game loop, such as whether the sound has finished playing.
    void Update();
};
