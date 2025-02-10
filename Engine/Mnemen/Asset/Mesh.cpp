//
// > Notice: Amélie Heinrich @ 2025
// > Create Time: 2025-02-03 23:37:55
//

#include "Mesh.hpp"
#include "Core/Logger.hpp"

#include <meshoptimizer.h>

#include <Asset/AssetManager.hpp>
#include <RHI/Uploader.hpp>

void Mesh::Load(RHI::Ref rhi, const String& path)
{
    mRHI = rhi;
    Path = path;
    Directory = path.substr(0, path.find_last_of('/'));

    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(path, aiProcess_FlipUVs | aiProcess_PreTransformVertices);
    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        LOG_ERROR("Failed to load model at path %s", path.c_str());
    }
    
    Root = new MeshNode;
    Root->Name = "RootNode";
    Root->Parent = nullptr;
    Root->Transform = glm::mat4(1.0f);
    ProcessNode(Root, scene->mRootNode, scene);
}

Mesh::~Mesh()
{
    FreeNodes(Root);
    Materials.clear();
}

void Mesh::FreeNodes(MeshNode* node)
{
    if (!node)
        return;

    for (MeshNode* child : node->Children) {
        FreeNodes(child);
    }
    node->Children.clear();

    delete node;
}

void Mesh::ProcessNode(MeshNode* node, aiNode *assimpNode, const aiScene *scene)
{
    for (int i = 0; i < assimpNode->mNumMeshes; i++) {
        aiMesh *mesh = scene->mMeshes[assimpNode->mMeshes[i]];
        glm::mat4 transform(1.0f);
        ProcessPrimitive(mesh, node, scene, transform);
    }

    for (int i = 0; i < assimpNode->mNumChildren; i++) {
        MeshNode* childNode = new MeshNode;
        childNode->Parent = node;
        childNode->Transform = glm::mat4(1.0f);
        childNode->Name = assimpNode->mName.C_Str();
        node->Children.push_back(childNode);

        ProcessNode(childNode, assimpNode->mChildren[i], scene);
    }
}

void Mesh::ProcessPrimitive(aiMesh *mesh, MeshNode* node, const aiScene *scene, glm::mat4 transform)
{
    MeshPrimitive out;

    Vector<Vertex> vertices = {};
    Vector<UInt32> indices = {};

    for (int i = 0; i < mesh->mNumVertices; i++) {
        Vertex vertex;

        vertex.Position = glm::vec3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);
        if (mesh->HasNormals()) {
            vertex.Normal = glm::vec3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);
        }
        if (mesh->mTextureCoords[0]) {
            vertex.UV = glm::vec2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);
        }
        
        vertices.push_back(vertex);
    }

    for (int i = 0; i < mesh->mNumFaces; i++) {
        aiFace face = mesh->mFaces[i];
        for (int j = 0; j < face.mNumIndices; j++)
            indices.push_back(face.mIndices[j]);
    }

    Vector<meshopt_Meshlet> meshlets = {};
    Vector<UInt32> meshletVertices = {};
    Vector<Uint8> meshletTriangles = {};
    Vector<MeshletBounds> meshletBounds = {};

    const UInt64 kMaxTriangles = MAX_MESHLET_TRIANGLES;
    const UInt64 kMaxVertices = MAX_MESHLET_VERTICES;
    const float kConeWeight = 0.0f;

    UInt64 maxMeshlets = meshopt_buildMeshletsBound(indices.size(), kMaxVertices, kMaxTriangles);

    meshlets.resize(maxMeshlets);
    meshletVertices.resize(maxMeshlets * kMaxVertices);
    meshletTriangles.resize(maxMeshlets * kMaxTriangles * 3);

    UInt64 meshletCount = meshopt_buildMeshlets(
            meshlets.data(),
            meshletVertices.data(),
            meshletTriangles.data(),
            reinterpret_cast<const UInt32*>(indices.data()),
            indices.size(),
            reinterpret_cast<const float*>(vertices.data()),
            vertices.size(),
            sizeof(Vertex),
            kMaxVertices,
            kMaxTriangles,
            kConeWeight);

    const meshopt_Meshlet& last = meshlets[meshletCount - 1];
    meshletVertices.resize(last.vertex_offset + last.vertex_count);
    meshletTriangles.resize(last.triangle_offset + ((last.triangle_count * 3 + 3) & ~3));
    meshlets.resize(meshletCount);

    for (auto& m : meshlets) {
        meshopt_optimizeMeshlet(&meshletVertices[m.vertex_offset], &meshletTriangles[m.triangle_offset], m.triangle_count, m.vertex_count);
    
        // Generate bounds
        meshopt_Bounds meshopt_bounds = meshopt_computeMeshletBounds(&meshletVertices[m.vertex_offset], &meshletTriangles[m.triangle_offset],
                                                                     m.triangle_count, &vertices[0].Position.x, vertices.size(), sizeof(Vertex));
        
        MeshletBounds bounds;
        memcpy(glm::value_ptr(bounds.Center), meshopt_bounds.center, sizeof(float) * 3);
        memcpy(glm::value_ptr(bounds.ConeApex), meshopt_bounds.cone_apex, sizeof(float) * 3);
        memcpy(glm::value_ptr(bounds.ConeAxis), meshopt_bounds.cone_axis, sizeof(float) * 3);

        bounds.Radius = meshopt_bounds.radius;
        bounds.ConeCutoff = meshopt_bounds.cone_cutoff;
        meshletBounds.push_back(bounds);
    }

    // PUSH
    Vector<UInt32> meshletPrimitives;
    for (auto& val : meshletTriangles) {
        meshletPrimitives.push_back(static_cast<UInt32>(val));
    }

    out.VertexCount = vertices.size();
    out.IndexCount = indices.size();
    out.MeshletCount = meshlets.size();

    out.VertexBuffer = mRHI->CreateBuffer(vertices.size() * sizeof(Vertex), sizeof(Vertex), BufferType::Vertex, node->Name + " Vertex Buffer");
    out.VertexBuffer->BuildSRV();
    out.VertexBuffer->Tag(ResourceTag::ModelGeometry);

    out.IndexBuffer = mRHI->CreateBuffer(indices.size() * sizeof(UInt32), sizeof(UInt32), BufferType::Index, node->Name + " Index Buffer");
    out.IndexBuffer->BuildSRV();
    out.IndexBuffer->Tag(ResourceTag::ModelGeometry);

    out.MeshletBuffer = mRHI->CreateBuffer(meshlets.size() * sizeof(meshopt_Meshlet), sizeof(meshopt_Meshlet), BufferType::Storage, node->Name + " Meshlet Buffer");
    out.MeshletBuffer->BuildSRV();
    out.MeshletBuffer->Tag(ResourceTag::ModelGeometry);

    out.MeshletVertices = mRHI->CreateBuffer(meshletVertices.size() * sizeof(UInt32), sizeof(UInt32), BufferType::Storage, node->Name + " Meshlet Vertices");
    out.MeshletVertices->BuildSRV();
    out.MeshletVertices->Tag(ResourceTag::ModelGeometry);

    out.MeshletTriangles = mRHI->CreateBuffer(meshletPrimitives.size() * sizeof(UInt32), sizeof(UInt32), BufferType::Storage, node->Name + " Meshlet Triangles");
    out.MeshletTriangles->BuildSRV();
    out.MeshletTriangles->Tag(ResourceTag::ModelGeometry);

    out.MeshletBounds = mRHI->CreateBuffer(meshletBounds.size() * sizeof(MeshletBounds), sizeof(MeshletBounds), BufferType::Storage, node->Name + " Meshlet Bounds");
    out.MeshletBounds->BuildSRV();
    out.MeshletBounds->Tag(ResourceTag::ModelGeometry);

    out.GeometryStructure = mRHI->CreateBLAS(out.VertexBuffer, out.IndexBuffer, out.VertexCount, out.IndexCount, node->Name + " BLAS");

    aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
    MeshMaterial meshMaterial = {};
    out.MaterialIndex = Materials.size();

    aiColor3D flatColor(1.0f, 1.0f, 1.0f);
    material->Get(AI_MATKEY_COLOR_DIFFUSE, flatColor);
    meshMaterial.MaterialColor = glm::vec3(flatColor.r, flatColor.g, flatColor.b);
    
    aiString str;
    material->GetTexture(aiTextureType_DIFFUSE, 0, &str);
    if (str.length) {
        String texturePath = Directory + '/' + str.C_Str();

        meshMaterial.Albedo = AssetManager::Get(texturePath, AssetType::Texture);
        meshMaterial.AlbedoView = mRHI->CreateView(meshMaterial.Albedo->Texture, ViewType::ShaderResource);
    }

    Uploader::EnqueueBufferUpload(vertices.data(), out.VertexBuffer->GetSize(), out.VertexBuffer);
    Uploader::EnqueueBufferUpload(indices.data(), out.IndexBuffer->GetSize(), out.IndexBuffer);
    Uploader::EnqueueBufferUpload(meshlets.data(), out.MeshletBuffer->GetSize(), out.MeshletBuffer);
    Uploader::EnqueueBufferUpload(meshletVertices.data(), out.MeshletVertices->GetSize(), out.MeshletVertices);
    Uploader::EnqueueBufferUpload(meshletPrimitives.data(), out.MeshletTriangles->GetSize(), out.MeshletTriangles);
    Uploader::EnqueueBufferUpload(meshletBounds.data(), out.MeshletBounds->GetSize(), out.MeshletBounds);

    VertexCount += out.VertexCount;
    IndexCount += out.IndexCount;

    Materials.push_back(meshMaterial);
    node->Primitives.push_back(out);
}
