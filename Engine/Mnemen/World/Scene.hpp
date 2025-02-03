//
// > Notice: Amélie Heinrich @ 2025
// > Create Time: 2025-02-03 22:44:35
//

#pragma once

#include "Entity.hpp"
#include "Components.hpp"

struct SceneCamera
{
    glm::mat4 View;
    glm::mat4 Projection;
};

class Scene
{
public:
    Scene();
    ~Scene();

    void Update();
    SceneCamera GetMainCamera();

    Entity* AddEntity(const String& name = "Sigma Entity");
    void RemoveEntity(Entity* e);
private:
    friend class Entity;
    friend class Renderer;

    entt::registry mRegistry;
    Vector<Entity*> mEntities;
};
