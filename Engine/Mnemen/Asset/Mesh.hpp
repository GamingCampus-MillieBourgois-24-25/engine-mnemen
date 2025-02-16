//
// > Notice: Amélie Heinrich @ 2025
// > Create Time: 2025-02-03 23:32:42
//

#pragma once

#include <Core/Common.hpp>
#include <RHI/RHI.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <Assimp/Importer.hpp>
#include <Assimp/scene.h>
#include <Assimp/postprocess.h>
#include <Assimp/pbrmaterial.h>

#define MAX_MESHLET_TRIANGLES 124
#define MAX_MESHLET_VERTICES 64

class Asset;

/// @struct Vertex
/// @brief Represents a single vertex in a mesh.
///
/// Contains position, texture coordinates, and normal vector.
struct Vertex
{
    glm::vec3 Position; ///< The position of the vertex.
    glm::vec2 UV;       ///< The texture coordinates.
    glm::vec3 Normal;   ///< The normal vector.
};

/// @struct MeshMaterial
/// @brief Represents material properties of a mesh.
///
/// Stores references to textures and material properties like color and transparency.
struct MeshMaterial
{
    Ref<Asset> Albedo; ///< Pointer to the albedo texture asset.
    View::Ref AlbedoView; ///< View pointer to the albedo texture.

    Ref<Asset> Normal; ///< Pointer to the normal texture asset.
    View::Ref NormalView; ///< View pointer to the normal texture.

    bool AlphaTested; ///< Indicates if the material uses alpha testing.
    float AlphaCutoff; ///< Cutoff threshold for alpha testing.

    glm::vec3 MaterialColor; ///< The base color of the material.
};

/// @struct MeshletBounds
/// @brief Represents the bounds of a meshlet.
///
/// Contains bounding sphere and normal cone for culling optimizations.
struct MeshletBounds
{
    glm::vec3 Center; ///< Center of the bounding sphere.
    float Radius; ///< Radius of the bounding sphere.

    glm::vec3 ConeApex; ///< Apex of the normal cone.
    glm::vec3 ConeAxis; ///< Axis of the normal cone.
    float ConeCutoff; ///< Cosine of the cone angle divided by 2.
};

/// @struct MeshPrimitive
/// @brief Represents a single drawable part of a mesh.
///
/// Stores buffer references and rendering-related data.
struct MeshPrimitive
{
    Buffer::Ref VertexBuffer; ///< Pointer to the vertex buffer.
    Buffer::Ref IndexBuffer; ///< Pointer to the index buffer.
    Buffer::Ref MeshletBuffer; ///< Pointer to the meshlet buffer.
    Buffer::Ref MeshletVertices; ///< Pointer to the meshlet vertices buffer.
    Buffer::Ref MeshletTriangles; ///< Pointer to the meshlet triangles buffer.
    Buffer::Ref MeshletBounds; ///< Pointer to the meshlet bounds buffer.

    RaytracingInstance Instance; ///< Instance for ray tracing.
    BLAS::Ref GeometryStructure; ///< Bottom-level acceleration structure for ray tracing.

    UInt32 VertexCount; ///< Number of vertices in the primitive.
    UInt32 IndexCount; ///< Number of indices in the primitive.
    UInt32 MeshletCount; ///< Number of meshlets in the primitive.
    int MaterialIndex; ///< Index of the material used by this primitive.
};

/// @struct MeshNode
/// @brief Represents a node in the mesh hierarchy.
///
/// Stores transformation data and child relationships.
struct MeshNode
{
    Vector<MeshPrimitive> Primitives; ///< List of mesh primitives in the node.
    Array<Buffer::Ref, FRAMES_IN_FLIGHT> ModelBuffer; ///< Per-frame model transformation buffers.

    String Name = ""; ///< Name of the node.
    glm::mat4 Transform; ///< Transformation matrix.
    MeshNode* Parent = nullptr; ///< Pointer to the parent node.
    Vector<MeshNode*> Children = {}; ///< List of child nodes.

    MeshNode() = default;
};

/// @class Mesh
/// @brief Represents a 3D mesh with materials and hierarchy.
///
/// Handles mesh loading, storage, and rendering-related data.
class Mesh
{
public:
    String Path; ///< File path of the mesh.
    String Directory; ///< Directory containing the mesh assets.

    MeshNode* Root = nullptr; ///< Root node of the mesh hierarchy.
    Vector<MeshMaterial> Materials; ///< List of materials used by the mesh.

    UInt32 VertexCount = 0; ///< Total vertex count in the mesh.
    UInt32 IndexCount = 0; ///< Total index count in the mesh.
    UInt32 MeshletCount = 0; ///< Total meshlet count in the mesh.

    /// @brief Loads a mesh from a file.
    /// @param rhi Pointer to the rendering hardware interface.
    /// @param path Path to the mesh file.
    void Load(RHI::Ref rhi, const String& path);

    /// @brief Destructor for Mesh, responsible for cleanup.
    ~Mesh();

private:
    RHI::Ref mRHI; ///< Pointer to the rendering hardware interface.

    /// @brief Processes a primitive within a mesh.
    /// @param mesh The Assimp mesh data.
    /// @param node The corresponding MeshNode.
    /// @param scene The Assimp scene pointer.
    /// @param transform Transformation matrix applied to the primitive.
    void ProcessPrimitive(aiMesh *mesh, MeshNode* node, const aiScene *scene, glm::mat4 transform);

    /// @brief Processes a mesh node from Assimp.
    /// @param node The current MeshNode being processed.
    /// @param assimpNode The Assimp node pointer.
    /// @param scene The Assimp scene pointer.
    void ProcessNode(MeshNode* node, aiNode *assimpNode, const aiScene *scene);

    /// @brief Recursively frees all nodes in the hierarchy.
    /// @param node The node to be freed.
    void FreeNodes(MeshNode* node);
};
