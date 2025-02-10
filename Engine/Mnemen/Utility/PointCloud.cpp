//
// > Notice: Amélie Heinrich @ 2025
// > Create Time: 2025-02-09 16:32:12
//

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <Core/Logger.hpp>

#include "PointCloud.hpp"

void ProcessNode(aiNode* node, const aiScene* scene, Vector<float>& vertices, Vector<UInt32>& indices)
{
    for (UInt32 i = 0; i < node->mNumMeshes; ++i) {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];

        // Offset to maintain global indices
        int indexOffset = vertices.size() / 3;

        // Extract vertices
        for (unsigned int j = 0; j < mesh->mNumVertices; ++j) {
            glm::vec3 pos(mesh->mVertices[j].x, mesh->mVertices[j].y, mesh->mVertices[j].z);

            // Flattened for Recast (XYZ format)
            vertices.push_back(pos.x);
            vertices.push_back(pos.y);
            vertices.push_back(pos.z);
        }

        // Extract indices (triangulated)
        for (UInt32 j = 0; j < mesh->mNumFaces; ++j) {
            aiFace face = mesh->mFaces[j];
            if (face.mNumIndices == 3) { // Ensure triangle
                indices.push_back(face.mIndices[0] + indexOffset);
                indices.push_back(face.mIndices[1] + indexOffset);
                indices.push_back(face.mIndices[2] + indexOffset);
            }
        }
    }

    // Recursively process child nodes
    for (UInt32 i = 0; i < node->mNumChildren; ++i) {
        ProcessNode(node->mChildren[i], scene, vertices, indices);
    }
}

PointCloud::PointCloud(const String& path)
{
    Load(path);
}

void PointCloud::Load(const String& path)
{
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_GenNormals | aiProcess_JoinIdenticalVertices);

    if (!scene || !scene->mRootNode || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE) {
        LOG_ERROR("Failed to load point cloud!");
    }

    ProcessNode(scene->mRootNode, scene, Points, Indices);
}
