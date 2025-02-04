//
// > Notice: Amélie Heinrich @ 2025
// > Create Time: 2025-02-03 22:44:35
//

#pragma once

#include "Entity.hpp"
#include "Components.hpp"

struct SceneCamera
{
    glm::mat4 View = glm::mat4(1.0f);
    glm::mat4 Projection = glm::mat4(1.0f);
};

class Scene
{
public:
    Scene();
    ~Scene();

    void Update();
    SceneCamera GetMainCamera();

    entt::registry* GetRegistry() { return &mRegistry; }
    Entity* AddEntity(const String& name = "Sigma Entity");
    void RemoveEntity(Entity* e);
private:
    friend class Entity;
    friend class Renderer;

    entt::registry mRegistry;
    Vector<Entity*> mEntities;
};
