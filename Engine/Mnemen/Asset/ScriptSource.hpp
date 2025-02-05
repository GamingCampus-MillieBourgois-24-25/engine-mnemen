//
// > Notice: Amélie Heinrich @ 2025
// > Create Time: 2025-02-04 23:11:33
//

#pragma once

#include <Core/Common.hpp>

/// @brief Represents the source code of a script.
/// @details The `ScriptSource` struct stores the script's source code as a string 
/// and provides a method to load the script from a file.
struct ScriptSource
{
    /// @brief The source code of the script.
    String Source;

    /// @brief Loads script source from a file.
    /// @param Path The file path to the script source.
    /// @details Reads the contents of the file and stores it in the `Source` string.
    void Load(const String& Path);
};

