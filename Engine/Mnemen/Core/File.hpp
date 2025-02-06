//
// > Notice: Amélie Heinrich @ 2025
// > Create Time: 2025-12-03 18:42:13
//

#pragma once

#include <nlohmann/json.hpp>

#include "Common.hpp"

/// @class File
/// @brief Provides utilities to handle the file system.
///
/// The File class is used for operations related to the file system -- reading, writing, creating, moving or copying files.
class File
{
public:
    /// @brief A structure representing a file time (last modified, last written, etc)
    struct Filetime
    {
        /// @brief The low part of the file time.
        UInt32 Low;

        /// @brief The high part of the file time.
        UInt32 High;

        /// @brief Equality operator to compare two filetime structures.
        /// @param other The other filetime to compare with.
        /// @return True if both low and high parts are equal, otherwise false.
        bool operator==(const Filetime& other) {
            return (Low == other.Low) && (High == other.High);
        }

        /// @brief Inequality operator to compare two filetime structures.
        /// @param other The other filetime to compare with.
        /// @return True if either the low or high parts are different, otherwise false.
        bool operator!=(const Filetime& other) {
            return !(*this == other);
        }
    };

    /// @brief Returns whether or not a file exists at the given path.
    /// @param path The path of the file.
    /// @return True if the file exists, otherwise false.
    static bool Exists(const String& path);

    /// @brief Check if the file entity at the given path is a directory.
    /// @param path The path of the file entity.
    /// @return True if the entity is a directory, otherwise false.
    static bool IsDirectory(const String& path);
    
    /// @brief Creates a file at the given path.
    /// @param path The path of the file to create.
    static void CreateFileFromPath(const String& path);

    /// @brief Creates a directory at the given path.
    /// @param path The path of the directory to create.
    static void CreateDirectoryFromPath(const String& path);

    /// @brief Deletes the file at the given path.
    /// @param path The path of the file to delete.
    static void Delete(const String& path);

    /// @brief Moves a file from the given old path to the new path.
    /// @param oldPath The old path of the file.
    /// @param newPath The new path of the file.
    static void Move(const String& oldPath, const String& newPath);
    
    /// @brief Copies a file from oldPath to newPath.
    /// @param oldPath The old path of the file.
    /// @param newPath The new path of the file.
    /// @param overwrite Whether to overwrite the file if it already exists.
    static void Copy(const String& oldPath, const String& newPath, bool overwrite = true);

    /// @brief Gets the extension of the given path.
    /// @param path The path of the file.
    /// @return The file extension (e.g., "photo.jpg" -> ".jpg").
    static String GetFileExtension(const String& path);

    /// @brief Returns the size of the file in bytes.
    /// @param path The path of the file.
    /// @return The size of the file in bytes.
    static Int32 GetFileSize(const String& path);

    /// @brief Reads the contents of a text file into a string.
    /// @param path The path of the file to read.
    /// @return A string containing the file's contents.
    static String ReadFile(const String& path);

    /// @brief Reads the contents of a binary file into an array of bytes.
    /// @param path The path of the file to read.
    /// @param data A memory buffer to hold the file's data.
    /// @param size The size of the memory buffer.
    static void ReadBytes(const String& path, void *data, UInt64 size);

    /// @brief Reads the contents of a binary file into an allocated array of bytes.
    /// @param path The path of the file to read.
    /// @return A pointer to an allocated array of bytes containing the file's contents.
    static void *ReadBytes(const String& path);

    /// @brief Writes the given array of bytes to the file at the given path.
    /// @param path The path of the file to write.
    /// @param data The data to write.
    /// @param size The size of the data to write.
    static void WriteBytes(const String& path, const void* data, UInt64 size);

    /// @brief Gets the last time a file was modified.
    /// @param path The path of the file to check.
    /// @return A `Filetime` representing the last modification time of the file.
    static Filetime GetLastModified(const String& path);

    static nlohmann::json LoadJSON(const String& path);
    static void WriteJSON(const nlohmann::json& json, const String& path);
};
