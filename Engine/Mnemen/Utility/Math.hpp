//
// > Notice: Amélie Heinrich @ 2025
// > Create Time: 2025-02-11 11:32:09
//

#pragma once

#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>

class Math
{
public:
    static glm::vec3 GetNormalizedPerpendicular(glm::vec3 base);
    static bool DecomposeTransform(const glm::mat4& transform, glm::vec3& translation, glm::vec3& rotation, glm::vec3& scale);
    static glm::quat EulerToQuat(glm::vec3 euler);
    static glm::vec3 QuatToEuler(glm::quat quat);
};
