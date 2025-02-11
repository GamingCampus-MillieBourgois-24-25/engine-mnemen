//
// > Notice: Amélie Heinrich @ 2025
// > Create Time: 2025-02-04 00:06:05
//

#include "Components.hpp"

void CameraComponent::Update(glm::vec3 Position, glm::vec3 Rotation)
{
    // Convert rotation angles from degrees to radians
    const float pitch = glm::radians(Rotation.z);
    const float yaw = glm::radians(Rotation.y);
    const float roll = glm::radians(Rotation.x);

    // Calculate rotation matrices for pitch, yaw, and roll
    glm::mat4 rotationMatrix = glm::mat4(1.0f);

    // Create the pitch (X-axis) rotation matrix
    glm::mat4 pitchMatrix = glm::rotate(glm::mat4(1.0f), pitch, glm::vec3(1.0f, 0.0f, 0.0f));
    
    // Create the yaw (Y-axis) rotation matrix
    glm::mat4 yawMatrix = glm::rotate(glm::mat4(1.0f), yaw, glm::vec3(0.0f, 1.0f, 0.0f));
    
    // Create the roll (Z-axis) rotation matrix
    glm::mat4 rollMatrix = glm::rotate(glm::mat4(1.0f), roll, glm::vec3(0.0f, 0.0f, 1.0f));

    // Combine all rotation matrices (Yaw -> Pitch -> Roll)
    rotationMatrix = rollMatrix * pitchMatrix * yawMatrix;

    // Calculate the front, right, and up vectors based on the rotation matrix
    glm::vec3 front = glm::normalize(glm::vec3(rotationMatrix[2]));  // Front is Z-axis
    glm::vec3 right = glm::normalize(glm::vec3(rotationMatrix[0]));  // Right is X-axis
    glm::vec3 up = glm::normalize(glm::vec3(rotationMatrix[1]));     // Up is Y-axis

    // Set the projection matrix for perspective (left-handed)
    Projection = glm::perspectiveLH(glm::radians(FOV), AspectRatio, Near, Far);

    // Set the view matrix using the lookAt function for the left-handed system
    View = glm::lookAtLH(Position, Position + front, up);
}

