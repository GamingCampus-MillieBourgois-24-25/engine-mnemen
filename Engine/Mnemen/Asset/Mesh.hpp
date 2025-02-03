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

struct Vertex
{
    glm::vec3 Position;
    glm::vec2 UV;
    glm::vec3 Normal;
};

struct MeshMaterial
{
    Ref<Asset> Albedo;
    View::Ref AlbedoView;

    Ref<Asset> Normal;
    View::Ref NormalView;

    bool AlphaTested;
    float AlphaCutoff;

    glm::vec3 MaterialColor;
};

struct MeshletBounds
{
    /* bounding sphere, useful for frustum and occlusion culling */
    glm::vec3 Center;
    float Radius;

    /* normal cone, useful for backface culling */
    glm::vec3 ConeApex;
    glm::vec3 ConeAxis;
    float ConeCutoff; /* = cos(angle/2) */
};

struct MeshPrimitive
{
    Buffer::Ref VertexBuffer;
    Buffer::Ref IndexBuffer;
    Buffer::Ref MeshletBuffer;
    Buffer::Ref MeshletVertices;
    Buffer::Ref MeshletTriangles;
    Buffer::Ref MeshletBounds;

    RaytracingInstance Instance;
    BLAS::Ref GeometryStructure;

    UInt32 VertexCount;
    UInt32 IndexCount;
    UInt32 MeshletCount;
    int MaterialIndex;
};

struct MeshNode
{
    Vector<MeshPrimitive> Primitives;
    Array<Buffer::Ref, FRAMES_IN_FLIGHT> ModelBuffer;

    String Name = "";
    glm::mat4 Transform;
    MeshNode* Parent = nullptr;
    Vector<MeshNode*> Children = {};

    MeshNode() = default;
};

class Mesh
{
public:
    String Path;
    String Directory;

    MeshNode* Root = nullptr;
    Vector<MeshMaterial> Materials;

    UInt32 VertexCount = 0;
    UInt32 IndexCount = 0;
    UInt32 MeshletCount = 0;

    void Load(RHI::Ref rhi, const String& path);
    ~Mesh();
private:
    RHI::Ref mRHI;

    void ProcessPrimitive(aiMesh *mesh, MeshNode* node, const aiScene *scene, glm::mat4 transform);
    void ProcessNode(MeshNode* node, aiNode *assimpNode, const aiScene *scene);
    void FreeNodes(MeshNode* node);
};
