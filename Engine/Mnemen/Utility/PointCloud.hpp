//
// > Notice: Amélie Heinrich @ 2025
// > Create Time: 2025-02-09 16:26:10
//

// @NOTE(Amélie) : This file is a utility for both PHYSICS and AI. Usually, when creating a navigation mesh or a convex hull collider, you can
// often do that when you're loading the mesh file. However, since an entity with a mesh might not have a collider or a navmesh, we need to load it again.
// Note that this version of the loader is *INSANELY* stripped down. Ideally, we'd want to cache geometry when loaded so that we can reuse it when
// a navmesh/physics collider is requested. Might be good to improve, but since we are not working with *millions* of vertices, we won't bother for now.
//
// HOWEVER: When using Recast or Jolt, it is recommended to cache the colliders and navmesh, and cache them using the model's path + a tag like "AI" or "Collider".

#pragma once

#include <Core/Common.hpp>
#include <glm/glm.hpp>
/// @brief A struct representing a point cloud.
class PointCloud
{
public:
    /// @brief The default constructor
    PointCloud() = default;
    /// @brief Initializes the point cloud and calls Load.
    /// @param path The path of the point cloud to load
    PointCloud(const String& path);
    /// @brief Default destructor
    ~PointCloud() = default;

    /// @brief The list of points
    Vector<float> Points;
    /// @brief The indices of the point cloud
    Vector<UInt32> Indices;
    
    /// @brief Load a point cloud from the given 3D mesh path.
    /// @param path The path of the mesh to load.
    void Load(const String& path);
};
