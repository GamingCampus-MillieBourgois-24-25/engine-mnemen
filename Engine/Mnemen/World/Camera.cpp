//
// > Notice: Amélie Heinrich @ 2025
// > Create Time: 2025-02-04 00:06:05
//

#include "Components.hpp"

void CameraComponent::Update(glm::vec3 Position, glm::vec3 Rotation)
{
    const float pitch = glm::radians(Rotation.x);
    const float yaw =   glm::radians(Rotation.y);

    glm::vec3 front;
    front.x = glm::sin(yaw);
    front.y = -(glm::sin(pitch) * glm::cos(yaw));
    front.z = -(glm::cos(pitch) * glm::cos(yaw));
    front = glm::normalize(front);

    Projection = glm::perspective(glm::radians(FOV), AspectRatio, Near, Far);
    View = glm::lookAt(Position, Position + front, glm::vec3(0.0f, 1.0f, 0.0f));
}
