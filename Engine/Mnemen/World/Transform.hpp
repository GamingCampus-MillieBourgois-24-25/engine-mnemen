//
// > Notice: Amélie Heinrich @ 2025
// > Create Time: 2025-02-03 22:58:16
//

#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

struct TransformComponent
{
    glm::vec3 Position = glm::vec3(0.0f);
    glm::vec3 Scale = glm::vec3(1.0f);
    glm::vec3 Rotation = glm::vec3(0.0f);
    glm::mat4 Matrix;

    void Update();
};
