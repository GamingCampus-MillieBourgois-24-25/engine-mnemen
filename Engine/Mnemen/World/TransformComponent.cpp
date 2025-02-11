//
// > Notice: Amélie Heinrich @ 2025
// > Create Time: 2025-02-03 22:59:40
//

#include "Components.hpp"

void TransformComponent::Update()
{
    Matrix = glm::translate(glm::mat4(1.0f), Position) 
           * glm::rotate(glm::mat4(1.0f), glm::radians(Rotation.z), glm::vec3(0.0f, 0.0f, 1.0f))
           * glm::rotate(glm::mat4(1.0f), glm::radians(Rotation.y), glm::vec3(0.0f, 1.0f, 0.0f))
           * glm::rotate(glm::mat4(1.0f), glm::radians(Rotation.x), glm::vec3(1.0f, 0.0f, 0.0f))
           * glm::scale(glm::mat4(1.0f), Scale);
}
