//
// > Notice: Amélie Heinrich @ 2025
// > Create Time: 2025-02-11 21:40:31
//

#pragma once

#include <Core/Common.hpp>

/// @class Dialog
/// @brief A utility class for opening and saving files through dialog boxes.
///
/// This class provides static methods for opening and saving files, using the specified
/// file extensions. It helps manage the file selection process for file input/output operations.
class Dialog
{
public:
    /// @brief Opens a file dialog to select a file with specific extensions.
    ///
    /// This function opens a file dialog where the user can select a file. The allowed file
    /// extensions are specified in the `extensions` parameter.
    ///
    /// @param extensions A vector of allowed file extensions (e.g., {"*.txt", "*.png"}).
    /// @return The selected file path as a string, or an empty string if no file is selected.
    static String Open(const Vector<String>& extensions);

    /// @brief Opens a save file dialog to choose a location for saving a file with specific extensions.
    ///
    /// This function opens a file dialog where the user can select a location to save a file.
    /// The allowed file extensions are specified in the `extensions` parameter.
    ///
    /// @param extensions A vector of allowed file extensions (e.g., {"*.txt", "*.png"}).
    /// @return The chosen file path to save the file, or an empty string if no location is selected.
    static String Save(const Vector<String>& extensions);
};

