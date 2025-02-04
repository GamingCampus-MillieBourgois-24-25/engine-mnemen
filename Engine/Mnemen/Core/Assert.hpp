//
// > Notice: Amélie Heinrich @ 2025
// > Create Time: 2025-12-03 05:53:03
//

#pragma once

#include <string>

#include "Common.hpp"

class Assert
{
public:
    /// @brief Checks if the given condition is true -- if false, crash the engine and show a debug window
    /// @param condition The condition to check
    /// @param fileName The file name
    /// @param function The function
    /// @param line The line
    /// @param message The error message to display if the condition is false
    static void Check(bool condition, const String& fileName, const String& function, int line, const String& message);
};

#define ASSERT(cond, msg) ::Assert::Check(cond, __FILE__, __FUNCTION__, __LINE__, msg)