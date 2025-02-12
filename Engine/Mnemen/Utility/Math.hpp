//
// > Notice: Amélie Heinrich @ 2025
// > Create Time: 2025-02-11 11:32:09
//

#pragma once

#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>

/// @class Math
/// @brief A utility class providing mathematical operations and transformations.
///
/// This class contains static methods for performing common mathematical operations,
/// such as getting a perpendicular vector, decomposing transformations, and converting
/// between Euler angles and quaternions.
class Math
{
public:
    /// @brief Returns a normalized perpendicular vector to the given base vector.
    ///
    /// This function computes a perpendicular vector to the input `base` vector
    /// and normalizes it to ensure it has a unit length.
    ///
    /// @param base The base vector to find the perpendicular of.
    /// @return A normalized perpendicular vector.
    static glm::vec3 GetNormalizedPerpendicular(glm::vec3 base);

    /// @brief Decomposes a transformation matrix into translation, rotation, and scale components.
    ///
    /// This function takes a 4x4 transformation matrix and decomposes it into separate
    /// translation, rotation, and scale components.
    ///
    /// @param transform The 4x4 transformation matrix to decompose.
    /// @param translation The resulting translation vector.
    /// @param rotation The resulting rotation vector (in Euler angles).
    /// @param scale The resulting scale vector.
    /// @return `true` if the decomposition is successful, `false` otherwise.
    static bool DecomposeTransform(const glm::mat4& transform, glm::vec3& translation, glm::vec3& rotation, glm::vec3& scale);

    /// @brief Converts Euler angles to a quaternion.
    ///
    /// This function converts a set of Euler angles (pitch, yaw, roll) to a quaternion.
    ///
    /// @param euler The Euler angles in degrees.
    /// @return The corresponding quaternion.
    static glm::quat EulerToQuat(glm::vec3 euler);

    /// @brief Converts a quaternion to Euler angles.
    ///
    /// This function converts a quaternion to a set of Euler angles (pitch, yaw, roll).
    ///
    /// @param quat The quaternion to convert.
    /// @return The corresponding Euler angles in degrees.
    static glm::vec3 QuatToEuler(glm::quat quat);
};
