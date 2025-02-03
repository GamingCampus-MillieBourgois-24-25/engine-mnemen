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
