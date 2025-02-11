#pragma once
#include <vector>
#include <glm/glm.hpp>

class NavAgent {
public:
    NavAgent(const glm::vec3& startPosition, float speed);

    void SetPath(const std::vector<glm::vec3>& newPath);
    void Update(float deltaTime);

    glm::vec3 GetPosition() const { return position; }
    bool HasReachedDestination() const;

private:
    glm::vec3 position;
    float speed;
    std::vector<glm::vec3> path;
    int currentTargetIndex;
};
