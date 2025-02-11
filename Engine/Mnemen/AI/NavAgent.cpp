#include "NavAgent.hpp"
#include <glm/gtx/norm.hpp> // Pour glm::distance2

NavAgent::NavAgent(const glm::vec3& startPosition, float speed)
    : position(startPosition), speed(speed), currentTargetIndex(0) {}

void NavAgent::SetPath(const std::vector<glm::vec3>& newPath) {
    path = newPath;
    currentTargetIndex = 0; // On commence au premier point
}

void NavAgent::Update(float deltaTime) {
    if (currentTargetIndex >= path.size()) return; // Plus de points à suivre

    glm::vec3 target = path[currentTargetIndex];
    glm::vec3 direction = glm::normalize(target - position);
    float distance = glm::length(target - position);
    
    float step = speed * deltaTime;
    
    if (step >= distance) {
        position = target;
        currentTargetIndex++;
    } else {
        position += direction * step;
    }
}

bool NavAgent::HasReachedDestination() const {
    return currentTargetIndex >= path.size();
}
