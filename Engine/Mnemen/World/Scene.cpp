//
// > Notice: Amélie Heinrich @ 2025
// > Create Time: 2025-02-03 22:48:12
//

#include "Scene.hpp"

Scene::Scene()
{
    
}

Scene::~Scene()
{
    auto view = mRegistry.view<TagComponent>();
    for (auto [id, tag] : view.each()) {
        Entity entity(&mRegistry);
        entity.ID = id;

        if (entity.HasComponent<AudioSourceComponent>()) {
            entity.GetComponent<AudioSourceComponent>().Free();
        }
        if (entity.HasComponent<MeshComponent>()) {
            entity.GetComponent<MeshComponent>().Free();
        }
        if (entity.HasComponent<CameraComponent>()) {
            entity.GetComponent<CameraComponent>().Free();
        }
        if (entity.HasComponent<MaterialComponent>()) {
            entity.GetComponent<MaterialComponent>().Free();
        }
        mRegistry.destroy(id);
    }
}

void Scene::Update()
{
    // Transform update
    {
        auto view = mRegistry.view<TransformComponent>();
        for (auto [entity, transform] : view.each()) {
            transform.Update();
        }
    }

    // Camera Update (to sync camera with transformations)
    {
        auto view = mRegistry.view<TransformComponent, CameraComponent>();
        for (auto [entity, transform, camera] : view.each()) {
            camera.Update(transform.Position, transform.Rotation);
        }
    }
}

CameraComponent* Scene::GetMainCamera()
{
    // NOTE(amelie): This is professional grade spaghetti bullshit but lowkey iterating through entities is fast as hell. Love EnTT x

    Vector<CameraComponent*> cameras;
    auto view = mRegistry.view<CameraComponent>();
    for (auto [entity, camera] : view.each()) {
        cameras.push_back(&camera);
    }

    std::sort(cameras.begin(), cameras.end(), [](CameraComponent* a, CameraComponent* b) {
        return a->Primary > b->Primary;
    });

    if (!cameras.empty()) {
        const auto& bestCamera = cameras.front();
        if (bestCamera->Primary > 0)
            return bestCamera;
    }
    return nullptr;
}

Entity Scene::AddEntity(const String& name)
{
    Entity newEntity(&mRegistry);

    newEntity.ID = mRegistry.create();
    newEntity.AddComponent<TransformComponent>();
    newEntity.AddComponent<ScriptComponent>();
    newEntity.AddComponent<TagComponent>().Tag = name;
    newEntity.AddComponent<ChildrenComponent>();

    return newEntity;
}

void Scene::RemoveEntity(Entity e)
{
    // Remove parent, if any
    if (e.HasParent())
        e.RemoveParent();

    // Remove children, if any
    auto& children = e.GetComponent<ChildrenComponent>().Children;
    for (Entity& child : children) {
        RemoveEntity(child);
    }

    // Cleanup entity data
    if (e.HasComponent<AudioSourceComponent>()) {
        e.GetComponent<AudioSourceComponent>().Free();
    }
    if (e.HasComponent<MeshComponent>()) {
        e.GetComponent<MeshComponent>().Free();
    }
    if (e.HasComponent<CameraComponent>()) {
        e.GetComponent<CameraComponent>().Free();
    }
    if (e.HasComponent<MaterialComponent>()) {
        e.GetComponent<MaterialComponent>().Free();
    }
    mRegistry.destroy(e.ID);
}
