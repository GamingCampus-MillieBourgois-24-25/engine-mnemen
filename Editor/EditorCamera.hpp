//
// > Notice: Amélie Heinrich @ 2025
// > Create Time: 2025-02-04 00:32:08
//

#pragma once

#include <Core/Common.hpp>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

constexpr float CAMERA_NEAR = 0.1f;
constexpr float CAMERA_FAR = 150.0f;

struct Plane
{
    glm::vec3 Normal;
    float Distance;
};

class Camera
{
public:
    Camera() = default;
    ~Camera() = default;

    void UpdateMatrices(int width, int height);
    void Input(float dt);

    glm::mat4 View() const { return mView; }
    glm::mat4 Projection() const { return mProjection; }
    glm::vec3 Position() const { return mPosition; }
    
    Vector<glm::vec4> Corners() const;
    Vector<glm::vec4> CornersForCascade(float near, float far) const;
    Array<Plane, 6> Planes() const;

    static Array<Plane, 6> FrustumPlanes(glm::mat4 projView);
    static Vector<glm::vec4> FrustumCorners(glm::mat4 view, glm::mat4 proj);

    void FreezeFrustum(bool freeze);
private:
    glm::mat4 mView = glm::mat4(1.0f);
    glm::mat4 mProjection = glm::mat4(1.0f);
    glm::vec3 mPosition = glm::vec3(0.0f, 0.0f, 1.0f);
    glm::vec3 mForward = glm::vec3(0.0f);
    glm::vec3 mUp = glm::vec3(0.0f);
    glm::vec3 mRight = glm::vec3(0.0f);

    // To calculate forward
    float mPitch = 0.0f;
    float mYaw = -90.0f;

    // Saved frustum
    bool mFreezeFrustum = false;
    Array<Plane, 6> mSavedFrustum;
    int mWidth;
    int mHeight;
};
