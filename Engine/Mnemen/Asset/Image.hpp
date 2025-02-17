//
// > Notice: Amélie Heinrich @ 2025
// > Create Time: 2025-12-04 02:27:24
//

#pragma once

#include <Core/Common.hpp>

/// @struct Image
/// @brief Represents an image with pixel data and metadata.
///
/// The Image structure holds information about an image, including its
/// dimensions, mip levels, compression status, and pixel data.
struct Image
{
    int Width;        ///< The width of the image in pixels.
    int Height;       ///< The height of the image in pixels.
    int Levels;       ///< Number of mip levels in the image.
    bool Compressed = false; ///< Indicates whether the image is compressed.
    Vector<UInt8> Pixels; ///< The raw pixel data of the image.

    /// @brief Loads an image from a file.
    /// @param path The file path of the image to load.
    void Load(const String& path);

    /// @brief Loads an HDR image from a file.
    /// @param path The file path of the HDR image to load.
    void LoadHDR(const String& path);
};
