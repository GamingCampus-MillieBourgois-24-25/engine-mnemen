//
// > Notice: Amélie Heinrich @ 2025
// > Create Time: 2025-02-11 11:32:37
//

#include <Utility/Math.hpp>

glm::vec3 Math::GetNormalizedPerpendicular(glm::vec3 base)
{
    if (abs(base.x) > abs(base.y)) {
        float len = sqrt(base.x * base.x + base.y * base.y);
        return glm::vec3(base.z, 0.0f, -base.x) / len;
    } else {
        float len = sqrt(base.y * base.y + base.z * base.z);
        return glm::vec3(0.0f, base.z, -base.y) / len;
    }
}
