//
// > Notice: Amélie Heinrich @ 2025
// > Create Time: 2025-02-04 12:03:29
//

#include "AISystem.hpp"
#include <Core/Logger.hpp>
#include <Core/Profiler.hpp>
#include <cmath>


AISystem::Data AISystem::sData;

void AISystem::Init()
{
    sData.Ctx = new rcContext(); // Création du contexte Recast
    memset(&sData.Config, 0, sizeof(sData.Config));

    // Initialisation du NavMesh Query
    sData.NavMesh = dtAllocNavMesh();
    if (!sData.NavMesh) {
        LOG_ERROR("Failed to allocate NavMesh");
        return;
    }

    sData.NavQuery = dtAllocNavMeshQuery();
    if (!sData.NavQuery) {
        LOG_ERROR("Failed to allocate NavMeshQuery");
        return;
    }

    LOG_INFO("NavMesh and NavMeshQuery allocated successfully");

    // Paramètres de configuration du NavMesh
    sData.Config.cs = 0.3f;   // Taille des cellules
    sData.Config.ch = 0.2f;   // Hauteur des cellules
    sData.Config.walkableSlopeAngle = 45.0f;  
    sData.Config.walkableHeight = (int)ceil(2.0f / sData.Config.ch);
    sData.Config.walkableClimb = (int)floor(0.9f / sData.Config.ch);
    sData.Config.walkableRadius = (int)ceil(0.6f / sData.Config.cs);
    sData.Config.maxEdgeLen = (int)(12.0f / sData.Config.cs);
    sData.Config.maxSimplificationError = 1.3f;
    sData.Config.minRegionArea = (int)rcSqr(8);
    sData.Config.mergeRegionArea = (int)rcSqr(20);
    sData.Config.maxVertsPerPoly = 6;
    sData.Config.detailSampleDist = 6.0f;
    sData.Config.detailSampleMaxError = 1.0f;
    
    LOG_INFO("Initialized AI system");
    
    if (dtStatusFailed(sData.NavQuery->init(sData.NavMesh, 2048))) { // 2048 is the number of polygon it's gonna search, so the bigger the number the longer the path can be.
        LOG_ERROR("Failed to initialize NavMeshQuery");
        return;
    }
    LOG_INFO("NavMeshQuery initialized successfully");
}

void AISystem::Exit()
{
    if (sData.NavQuery) {
        dtFreeNavMeshQuery(sData.NavQuery);
        sData.NavQuery = nullptr;
    }

    if (sData.NavMesh) {
        dtFreeNavMesh(sData.NavMesh);
        sData.NavMesh = nullptr;
    }

    if (sData.Ctx) {
        delete sData.Ctx;
        sData.Ctx = nullptr;
    }

    LOG_INFO("AI System exited and resources freed.");
}


void AISystem::Update(Ref<Scene> scene) {
    float deltaTime = scene->GetDeltaTime(); // Temps écoulé entre les frames

    for (auto& agent : agents) {
        agent.Update(deltaTime);
    }
}


void AISystem::BuildNavMesh(const std::vector<float>& verts, const std::vector<int>& indices)
{
    if (verts.empty() || indices.empty()) {
        LOG_ERROR("Verts or indices array is empty, cannot build NavMesh");
        return;
    }

    rcHeightfield* heightfield = rcAllocHeightfield();
    if (!heightfield) {
        LOG_ERROR("Failed to allocate heightfield");
        return;
    }

    if (!rcCreateHeightfield(sData.Ctx, *heightfield, sData.Config.width, sData.Config.height, sData.Config.bmin, sData.Config.bmax, sData.Config.cs, sData.Config.ch)) {
        LOG_ERROR("Failed to create heightfield");
        rcFreeHeightField(heightfield);
        return;
    }

    std::vector<unsigned char> triAreas(indices.size() / 3, 0);
    rcMarkWalkableTriangles(sData.Ctx, sData.Config.walkableSlopeAngle, verts.data(), verts.size() / 3, indices.data(), indices.size() / 3, triAreas.data());

    if (!rcRasterizeTriangles(sData.Ctx, verts.data(), verts.size() / 3, indices.data(), triAreas.data(), indices.size() / 3, *heightfield)) {
        LOG_ERROR("Failed to rasterize triangles");
        rcFreeHeightField(heightfield);
        return;
    }

    rcCompactHeightfield* compactHeightfield = rcAllocCompactHeightfield();
    if (!compactHeightfield) {
        LOG_ERROR("Failed to allocate compact heightfield");
        rcFreeHeightField(heightfield);
        return;
    }

    if (!rcBuildCompactHeightfield(sData.Ctx, sData.Config.walkableHeight, sData.Config.walkableClimb, *heightfield, *compactHeightfield)) {
        LOG_ERROR("Failed to build compact heightfield");
        rcFreeHeightField(heightfield);
        rcFreeCompactHeightfield(compactHeightfield);
        return;
    }

    rcContourSet* contourSet = rcAllocContourSet();
    if (!contourSet) {
        LOG_ERROR("Failed to allocate contour set");
        rcFreeHeightField(heightfield);
        rcFreeCompactHeightfield(compactHeightfield);
        return;
    }

    rcPolyMesh* polyMesh = rcAllocPolyMesh();
    if (!polyMesh) {
        LOG_ERROR("Failed to allocate poly mesh");
        rcFreeHeightField(heightfield);
        rcFreeCompactHeightfield(compactHeightfield);
        rcFreeContourSet(contourSet);
        return;
    }

    rcPolyMeshDetail* polyMeshDetail = rcAllocPolyMeshDetail();
    if (!polyMeshDetail) {
        LOG_ERROR("Failed to allocate poly mesh detail");
        rcFreeHeightField(heightfield);
        rcFreeCompactHeightfield(compactHeightfield);
        rcFreeContourSet(contourSet);
        rcFreePolyMesh(polyMesh);
        return;
    }

    dtNavMeshCreateParams params = {};
    params.verts = polyMesh->verts;
    params.vertCount = polyMesh->nverts;
    params.polys = polyMesh->polys;
    params.polyCount = polyMesh->npolys;
    params.nvp = polyMesh->nvp;
    params.detailMeshes = polyMeshDetail->meshes;
    params.detailVerts = polyMeshDetail->verts;
    params.detailVertsCount = polyMeshDetail->nverts;
    params.detailTris = polyMeshDetail->tris;
    params.detailTriCount = polyMeshDetail->ntris;
    params.walkableHeight = 2.0f;
    params.walkableRadius = 0.6f;
    params.walkableClimb = 0.9f;
    rcVcopy(params.bmin, polyMesh->bmin);
    rcVcopy(params.bmax, polyMesh->bmax);
    
    unsigned char* navData = nullptr;
    int navDataSize = 0;
    if (!dtCreateNavMeshData(&params, &navData, &navDataSize)) {
        LOG_ERROR("Failed to create NavMesh data");
        return;
    }

    sData.NavMesh = dtAllocNavMesh();
    if (!sData.NavMesh || dtStatusFailed(sData.NavMesh->init(navData, navDataSize, DT_TILE_FREE_DATA))) {
        LOG_ERROR("Failed to initialize NavMesh");
        dtFree(navData);
        return;
    }

    LOG_INFO("NavMesh successfully built!");
}


std::vector<glm::vec3> AISystem::FindPath(const glm::vec3& start, const glm::vec3& end)
{
    std::vector<glm::vec3> path;
    
    if (!sData.NavQuery || !sData.NavMesh) {
        LOG_ERROR("NavMeshQuery or NavMesh is not initialized");
        return path;
    }

    dtPolyRef startRef, endRef;
    float startNearest[3], endNearest[3];

    dtQueryFilter filter;
    filter.setIncludeFlags(0xFFFF); // Inclure toutes les surfaces navigables

    sData.NavQuery->findNearestPoly(glm::value_ptr(start), glm::value_ptr(glm::vec3(0.5f, 2.0f, 0.5f)), &filter, &startRef, startNearest);
    sData.NavQuery->findNearestPoly(glm::value_ptr(end), glm::value_ptr(glm::vec3(0.5f, 2.0f, 0.5f)), &filter, &endRef, endNearest);

    if (!startRef || !endRef) {
        LOG_ERROR("Failed to find start or end reference in NavMesh");
        return path;
    }

    dtPolyRef pathPolys[256];
    int pathCount;
    sData.NavQuery->findPath(startRef, endRef, startNearest, endNearest, &filter, pathPolys, &pathCount, 256);

    if (pathCount == 0) {
        LOG_WARN("No path found between given points");
        return path;
    }

    float straightPath[256 * 3];
    unsigned char straightPathFlags[256];
    dtPolyRef straightPathRefs[256];
    int straightPathCount;

    sData.NavQuery->findStraightPath(startNearest, endNearest, pathPolys, pathCount, straightPath, straightPathFlags, straightPathRefs, &straightPathCount, 256);

    for (int i = 0; i < straightPathCount; i++) {
        path.emplace_back(straightPath[i * 3], straightPath[i * 3 + 1], straightPath[i * 3 + 2]);
    }

    return path;
}

void AISystem::MoveAgent(NavAgent& agent, const glm::vec3& destination) {
    std::vector<glm::vec3> path = FindPath(agent.GetPosition(), destination);
    
    if (!path.empty()) {
        agent.SetPath(path);
    } else {
        LOG_WARNING("No valid path found for agent!");
    }
}
