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
    for (auto& entity : mEntities) {
        mRegistry.destroy(entity->ID);
        delete entity;
    }
    mEntities.clear();
}

void Scene::Update()
{
    // Transform update
    {
        for (auto [entity, transform] : mRegistry.view<TransformComponent>().each()) {
            transform.Update();
        }
    }

    // Camera Update
    {
        auto view = mRegistry.view<TransformComponent, CameraComponent>();
        for (auto [entity, transform, camera] : view.each()) {
            camera.Update(transform.Position, transform.Rotation);
        }
    }
}

SceneCamera Scene::GetMainCamera()
{
    for (auto& entity : mEntities) {
        if (entity->HasComponent<CameraComponent>()) {
            CameraComponent& camera = entity->GetComponent<CameraComponent>();
            if (camera.Primary) {
                return { camera.View, camera.Projection };
            }
        }
    }
    return { glm::mat4(1.0f), glm::mat4(1.0f) };
}

Entity* Scene::AddEntity(const String& name)
{
    Entity* newEntity = new Entity(&mRegistry);
    newEntity->ID = mRegistry.create();
    newEntity->Name = name;
    
    newEntity->AddComponent<TransformComponent>();

    mEntities.push_back(newEntity);
    return newEntity;
}

void Scene::RemoveEntity(Entity* e)
{
    for (UInt64 i = 0; i < mEntities.size(); i++) {
        if (e->ID == mEntities[i]->ID) {
            mEntities.erase(mEntities.begin() + i);
            delete e;
        }
    }
}
