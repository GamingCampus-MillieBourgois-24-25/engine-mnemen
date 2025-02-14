//
// > Notice: Amélie Heinrich @ 2025
// > Create Time: 2025-02-04 12:02:32
//

#pragma once
#include <Recast.h>
#include <DetourNavMesh.h>
#include <DetourNavMeshBuilder.h>
#include <DetourNavMeshQuery.h>
#include <vector>
#include "World/Scene.hpp"

/// @class AISystem
/// @brief A system responsible for AI-related processing.
///
/// The AISystem class provides static methods for initializing, exiting,
/// and updating AI-related functionalities within a scene.

class AISystem
{
public:
    /// @brief Initializes the AI system.
    static void Init();
    
    /// @brief Shuts down the AI system.
    static void Exit();

    /// @brief Updates the AI system for the given scene.
    /// 
    /// @param scene The scene to update AI components in.
    static void Update(Ref<Scene> scene);

    static std::vector<glm::vec3> FindPath(const glm::vec3& start, const glm::vec3& end);

    // static void MoveAgent(NavAgent& agent, const glm::vec3& destination);


private:
    struct Data {
        rcConfig Config;          // Configuration du NavMesh
        rcContext* Ctx;           // Contexte Recast
        dtNavMesh* NavMesh;       // Maillage de navigation
        dtNavMeshQuery* NavQuery; // Objet pour interroger le NavMesh
    };
    static Data sData;

    static void BuildNavMesh(const std::vector<float>& verts, const std::vector<int>& indices);
};