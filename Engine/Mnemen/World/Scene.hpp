//
// > Notice: Amélie Heinrich @ 2025
// > Create Time: 2025-02-03 22:44:35
//

#pragma once

#include "Entity.hpp"
#include "Transform.hpp"

class Scene
{
public:
    Scene();
    ~Scene();

    Entity* AddEntity(const String& name = "Sigma Entity");
    void RemoveEntity(Entity* e);
private:
    friend class Entity;

    entt::registry mRegistry;
    Vector<Entity*> mEntities;
};
