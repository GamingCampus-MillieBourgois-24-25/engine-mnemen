//
// > Notice: Amélie Heinrich @ 2024
// > Create Time: 2024-12-07 17:18:53
//

#include <Core/Random.hpp>

#include <random>

float Random::Float(float min, float max)
{
    static std::random_device rd;
    static std::mt19937 gen(rd());

    std::uniform_real_distribution<float> dis(min, max);
    return dis(gen);
}