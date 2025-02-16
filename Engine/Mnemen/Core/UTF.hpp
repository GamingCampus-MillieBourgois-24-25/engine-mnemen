//
// > Notice: Amélie Heinrich @ 2025
// > Create Time: 2025-12-03 06:20:19
//

#pragma once

#include "Common.hpp"

/// @brief A utility class for converting between wide and ASCII strings.
/// 
/// The `UTF` class provides static methods for converting strings between wide-character 
/// (UTF-16/UTF-32) and ASCII encodings. It supports both raw pointer and standard string 
/// types for input and output.
class UTF
{
public:
    /// @brief Converts a wide string (wchar_t*) to an ASCII string.
    /// 
    /// This method converts a string from wide characters (wchar_t) to an ASCII-encoded string.
    /// 
    /// @param text The wide string to convert.
    /// @return The converted ASCII string.
    static String WideToAscii(const wchar_t* text);

    /// @brief Converts a wide string (WideString) to an ASCII string.
    /// 
    /// This method converts a wide string (WideString) to an ASCII-encoded string.
    /// 
    /// @param text The wide string to convert.
    /// @return The converted ASCII string.
    static String WideToAscii(const WideString& text);

    /// @brief Converts an ASCII string (char*) to a wide string.
    /// 
    /// This method converts a string from ASCII encoding to a wide-character string (WideString).
    /// 
    /// @param text The ASCII string to convert.
    /// @return The converted wide string.
    static WideString AsciiToWide(const char* text);

    /// @brief Converts an ASCII string (String) to a wide string.
    /// 
    /// This method converts an ASCII string (String) to a wide-character string (WideString).
    /// 
    /// @param text The ASCII string to convert.
    /// @return The converted wide string.
    static WideString AsciiToWide(const String& text);
};