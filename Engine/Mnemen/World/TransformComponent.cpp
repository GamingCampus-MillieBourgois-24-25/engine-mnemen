//
// > Notice: Amélie Heinrich @ 2025
// > Create Time: 2025-02-03 22:59:40
//

#include "Entity.hpp"

void TransformComponent::Update()
{
    Matrix = glm::translate(glm::mat4(1.0f), Position)
           * glm::toMat4(Rotation) 
           * glm::scale(glm::mat4(1.0f), Scale);
}
