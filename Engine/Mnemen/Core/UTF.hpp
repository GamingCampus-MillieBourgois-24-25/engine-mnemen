//
// > Notice: Amélie Heinrich @ 2025
// > Create Time: 2025-12-03 06:20:19
//

#pragma once

#include "Common.hpp"

class UTF
{
public:
    /// @brief Converts a string from wide to ascii
    static String WideToAscii(const wchar_t* text);
    /// @brief Converts a string from wide to ascii
    static String WideToAscii(const WideString& text);
    /// @brief Converts a string from ascii to wide
    static WideString AsciiToWide(const char* text);
    /// @brief Converts a string from ascii to wide
    static WideString AsciiToWide(const String& text);
};