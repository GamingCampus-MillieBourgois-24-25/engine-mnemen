//
// > Notice: Amélie Heinrich @ 2024
// > Create Time: 2024-12-07 17:18:29
//

#pragma once

#include <Core/Common.hpp>

class Random
{
public:
    /// @brief Generates a random floating point number between min and max
    /// @param min Min
    /// @param max Max
    /// @return Random value between [min, max]
    static float Float(float min, float max);
};