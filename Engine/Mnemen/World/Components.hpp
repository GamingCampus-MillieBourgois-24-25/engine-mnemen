//
// > Notice: Amélie Heinrich @ 2025
// > Create Time: 2025-02-04 00:01:27
//

#pragma once

#include <Asset/AssetManager.hpp>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <Script/Script.hpp>

/// @brief A component representing a transform -- the spatial representation of the object.
struct TransformComponent
{
    /// @brief The position of the object
    glm::vec3 Position = glm::vec3(0.0f);
    /// @brief The scale of the object
    glm::vec3 Scale = glm::vec3(1.0f);
    /// @brief The rotation of the object
    glm::vec3 Rotation = glm::vec3(0.0f);
    /// @brief The matrix that compiles all the information
    glm::mat4 Matrix;

    /// @brief Updates the transform matrix
    void Update();
};

/// @brief A component holding a mesh
struct MeshComponent
{
    /// @brief The asset handle pointing to the mesh
    Asset::Handle MeshAsset;

    /// @brief Initializes the component and loads the mesh at the given string
    /// @param string The path of the component
    void Init(const String& string);

    ~MeshComponent();
};

/// @brief A component holding camera information
struct CameraComponent
{
    /// @brief If primary, it will be the the camera used by the scne
    bool Primary = true;
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
    void Update(glm::vec3 Position, glm::vec3 Rotation);
};

/// @brief A component holding a game script
struct ScriptComponent
{
    /// @brief An instance of a script
    struct Instance
    {
        /// @brief The path of the script
        String Path;
        /// @brief The handle of the script
        Script Handle;

        /// @brief Default constructor
        Instance() = default;
        /// @brief Default destructor
        ~Instance() = default;
    };

    /// @brief All attached instances of the script
    Vector<Ref<Instance>> Instances;

    /// @brief Pushes a script from the given path
    /// @param path The path of the script
    void PushScript(const String& path);
};
