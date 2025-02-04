//
// > Notice: Amélie Heinrich @ 2025
// > Create Time: 2025-12-07 17:18:29
//

#pragma once

#include "Common.hpp"

/// @brief A utility class for generating random values.
/// 
/// This class provides static methods for generating random numbers of various types,
/// including floating-point numbers in a specified range. The randomness is typically 
/// based on a pseudo-random number generator.
class Random
{
public:
    /// @brief Generates a random floating point number between min and max.
    /// 
    /// This method generates a random floating-point number between the provided 
    /// `min` and `max` values, inclusive.
    /// 
    /// @param min The minimum value for the generated random number.
    /// @param max The maximum value for the generated random number.
    /// @return A random floating-point number between [min, max].
    static float Float(float min, float max);
};