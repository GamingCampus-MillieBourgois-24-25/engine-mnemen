//
// > Notice: Amélie Heinrich @ 2025
// > Create Time: 2025-12-03 18:42:13
//

#pragma once

#include "Common.hpp"

class File
{
public:
    /// @brief A structure representing a file time (last modified, last written, etc)
    struct Filetime
    {
        UInt32 Low;
        UInt32 High;

        bool operator==(const Filetime& other) {
            return (Low == other.Low) && (High == other.High);
        }

        bool operator!=(const Filetime& other) {
            return !(*this == other);
        }
    };

    /// @brief Returns whether or not a file exist
    /// @param path The path of the file
    /// @return True if it exists, otherwise false
    static bool Exists(const String& path);

    /// @brief Check if the file entity at the given path is a directory
    /// @param path The path of the file entity
    /// @return True if it's a path -- otherwise false
    static bool IsDirectory(const String& path);
    
    /// @brief Creates a file at the given path
    /// @param path The path of the file to create
    static void CreateFileFromPath(const String& path);

    /// @brief Creates a directory at the given path
    /// @param path The path of the directory to create
    static void CreateDirectoryFromPath(const String& path);

    /// @brief Deletes the file at the given path
    /// @param path The path of the file to delete
    static void Delete(const String& path);

    /// @brief Moves a file from the given old path to the new path
    /// @param oldPath The old path of the file
    /// @param newPath The new path of the file
    static void Move(const String& oldPath, const String& newPath);
    
    /// @brief Copies a file from oldPath to newPath
    /// @param oldPath The old path of the file
    /// @param newPath The new path of the file
    /// @param overwrite Whether to overwrite the file if it already exists
    static void Copy(const String& oldPath, const String& newPath, bool overwrite = true);

    /// @brief Gets the extension of the given path
    /// @param path The path of the file
    /// @return "photo.jpg" -> ".jpg" for example
    static String GetFileExtension(const String& path);

    /// @brief Returns the size of the file in bytes
    /// @param path The path of the file
    /// @return The size of the file in bytes
    static Int32 GetFileSize(const String& path);

    /// @brief Reads the contents of a text file into a string
    /// @param path The path of the file to read
    /// @return The string of the file's contents
    static String ReadFile(const String& path);

    /// @brief Reads the contents of a binary file into an array of bytes
    /// @param path The path of the file to read
    /// @param data A memory buffer of the given size
    /// @param size The size of the memory buffer
    static void ReadBytes(const String& path, void *data, UInt64 size);

    /// @brief Reads the contents of a binary file into an allocated array of bytes
    /// @param path The path of the file to read
    /// @return An allocated array of bytes containing the file's contents
    static void *ReadBytes(const String& path);

    /// @brief Writes the given array of bytes to the file at the given path
    /// @param path The path of the file to write
    /// @param data The data to write of the given size
    /// @param size The size of the data to write
    static void WriteBytes(const String& path, const void* data, UInt64 size);

    /// @brief Gets the last time a file was modified
    /// @param path The path of the file to check
    /// @return A filetime of when it was last modified
    static Filetime GetLastModified(const String& path);
};