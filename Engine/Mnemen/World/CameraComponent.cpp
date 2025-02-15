//
// > Notice: Amélie Heinrich @ 2025
// > Create Time: 2025-02-04 00:06:05
//

#include "Entity.hpp"

#include <Core/Application.hpp>

CameraComponent::CameraComponent(bool load)
{
    if (load)
        Load("Assets/Volumes/DefaultVolume.mfx");
}

void CameraComponent::Load(const String& path)
{
    Free();
    Volume = AssetManager::Get(path, AssetType::PostFXVolume);
}

void CameraComponent::Free()
{
    if (Volume) {
        AssetManager::GiveBack(Volume->Path);
    }
}

void CameraComponent::Update(glm::vec3 Position, glm::quat Rotation)
{
    int width, height;
    Application::Get()->GetWindow()->PollSize(width, height);

    // Convert rotation angles from degrees to radians
    const float pitch = glm::radians(Rotation.y); // X-axis (Pitch)
    const float yaw = glm::radians(Rotation.x);   // Y-axis (Yaw)
    const float roll = glm::radians(Rotation.z);  // Z-axis (Roll)

    // Combine all rotation matrices (Yaw -> Pitch -> Roll) in DirectX order
    glm::mat4 rotationMatrix = glm::toMat4(Rotation);

    // Calculate the front, right, and up vectors based on the rotation matrix
    glm::vec3 front = glm::normalize(glm::vec3(rotationMatrix[2]));  // Front is Z-axis in DirectX
    glm::vec3 right = glm::normalize(glm::vec3(rotationMatrix[0]));  // Right is X-axis in DirectX
    glm::vec3 up = glm::normalize(glm::vec3(rotationMatrix[1]));     // Up is Y-axis in DirectX

    // Set the projection matrix for perspective (left-handed)
    Projection = glm::perspective(glm::radians(FOV), (float)width / (float)height, Near, Far);

    // Set the view matrix using the lookAt function for the left-handed system
    View = glm::lookAt(Position, Position + front, up);
}
