//
// > Notice: Amélie Heinrich @ 2025
// > Create Time: 2025-12-07 17:18:29
//

#pragma once

#include "Common.hpp"

class Random
{
public:
    /// @brief Generates a random floating point number between min and max
    /// @param min Min
    /// @param max Max
    /// @return Random value between [min, max]
    static float Float(float min, float max);
};