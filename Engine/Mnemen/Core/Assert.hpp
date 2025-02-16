//
// > Notice: Amélie Heinrich @ 2025
// > Create Time: 2025-12-03 05:53:03
//

#pragma once

#include <string>

#include "Common.hpp"

/// @class Assert
/// @brief Provides assertion functionality for debugging.
///
/// The Assert class is used to verify conditions during runtime and trigger
/// an error message if a condition fails, potentially crashing the engine.
class Assert
{
public:
    /// @brief Checks if the given condition is true; if false, crashes the engine and displays a debug window.
    /// @param condition The condition to check.
    /// @param fileName The name of the file where the assertion occurred.
    /// @param function The function where the assertion occurred.
    /// @param line The line number where the assertion occurred.
    /// @param message The error message to display if the condition is false.
    static void Check(bool condition, const String& fileName, const String& function, int line, const String& message);
};

/// @brief Macro for performing runtime assertions.
/// @param cond The condition to evaluate.
/// @param msg The message to display if the assertion fails.
#define ASSERT(cond, msg) ::Assert::Check(cond, __FILE__, __FUNCTION__, __LINE__, msg)