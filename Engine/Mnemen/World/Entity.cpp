//
// > Notice: Amélie Heinrich @ 2025
// > Create Time: 2025-02-11 16:24:24
//

#include "Entity.hpp"

#include <Utility/Math.hpp>

void Entity::SetParent(Entity parent)
{
    if (parent.ID == ID) {
        LOG_WARN("Entity cannot be its own parent!");
        return;
    }
    if (HasParent()) {
        RemoveParent();
    }

    glm::mat4 currentWorldTransform = GetWorldTransform();
    glm::mat4 parentWorldTransform = parent.GetWorldTransform();
    glm::mat4 newLocalTransform = glm::inverse(parentWorldTransform) * currentWorldTransform;
    SetLocalTransform(newLocalTransform);

    AddComponent<ParentComponent>(parent);
    parent.GetComponent<ChildrenComponent>().Children.push_back(*this);
}

bool Entity::HasParent()
{
    return HasComponent<ParentComponent>();
}

void Entity::RemoveParent()
{
    if (!HasParent())
        return;

    Entity parent = GetParent();
    SetLocalTransform(GetWorldTransform());

    auto& children = parent.GetComponent<ChildrenComponent>().Children;
    auto it = std::find_if(children.begin(), children.end(), [this](const Entity& child) {
        return child.ID == ID;
    });
    if (it != children.end()) {
        children.erase(it);
    }

    RemoveComponent<ParentComponent>();
}

Entity Entity::GetParent()
{
    if (!HasParent())
        return nullptr;
    return GetComponent<ParentComponent>().Parent;
}

Vector<Entity> Entity::GetChildren()
{
    return GetComponent<ChildrenComponent>().Children;
}

glm::mat4 Entity::GetWorldTransform()
{
    if (HasParent()) {
        Entity parentEntity = GetParent();
        return parentEntity.GetWorldTransform() * GetLocalTransform();
    }
    return GetLocalTransform();
}

glm::mat4 Entity::GetLocalTransform()
{
    return GetComponent<TransformComponent>().Matrix;
}

void Entity::SetLocalTransform(glm::mat4 localTransform)
{
    if (HasComponent<TransformComponent>()) {
        auto& tc = GetComponent<TransformComponent>(); 
        tc.Matrix = localTransform;

        glm::vec3 rotation;
        Math::DecomposeTransform(localTransform, tc.Position, rotation, tc.Scale);
        tc.Rotation = Math::EulerToQuat(rotation);
    }
}
