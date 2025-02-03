//
// > Notice: Amélie Heinrich @ 2025
// > Create Time: 2025-02-03 22:59:40
//

#include "Transform.hpp"

void TransformComponent::Update()
{
    Matrix = glm::translate(glm::mat4(1.0f), Position)
           * glm::scale(glm::mat4(1.0f), Scale)
           * glm::rotate(glm::mat4(1.0f), Rotation.x, glm::vec3(1.0f, 0.0f, 0.0f))
           * glm::rotate(glm::mat4(1.0f), Rotation.y, glm::vec3(0.0f, 1.0f, 0.0f))
           * glm::rotate(glm::mat4(1.0f), Rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));
}
