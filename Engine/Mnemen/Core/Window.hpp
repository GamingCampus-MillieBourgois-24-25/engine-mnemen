//
// > Notice: Amélie Heinrich @ 2025
// > Create Time: 2025-02-03 12:51:54
//

#pragma once

#include <SDL3/SDL.h>
#include "Common.hpp"

/// @brief A class representing a window in the application.
/// 
/// The `Window` class provides functionality for creating, updating, and querying the state 
/// of a window, as well as retrieving platform-specific handles such as Win32 and SDL handles.
class Window
{
public:
    /// @brief Creates a window with the specified dimensions and title.
    /// 
    /// This constructor initializes the window with a given width, height, and title. It also
    /// sets up necessary resources to make the window functional.
    /// 
    /// @param width The width of the window in pixels.
    /// @param height The height of the window in pixels.
    /// @param title The title of the window.
    Window(int width, int height, const String& title);

    /// @brief Destroys the window and cleans up resources.
    /// 
    /// This destructor cleans up any resources used by the window, such as SDL resources, and 
    /// ensures proper shutdown.
    ~Window();

    /// @brief Checks whether the window is open and running.
    /// 
    /// This method returns whether the window is still open and running.
    /// 
    /// @return `true` if the window is open, otherwise `false`.
    bool IsOpen() const { return mRunning; }

    /// @brief Updates the window, processing events and rendering.
    /// 
    /// This method should be called every frame to update the window. It processes any events 
    /// and renders the content if necessary.
    void Update();

    /// @brief Retrieves the size of the window.
    /// 
    /// This method retrieves the current size of the window and assigns it to the provided 
    /// width and height variables.
    /// 
    /// @param width Reference to the variable that will hold the window's width.
    /// @param height Reference to the variable that will hold the window's height.
    void PollSize(int& width, int& height);

    /// @brief Retrieves the Win32 handle of the window.
    /// 
    /// This method returns the platform-specific Win32 handle for the window.
    /// 
    /// @return The Win32 handle for the window.
    void* GetHandle();

    /// @brief Closes the window
    void Close();

    /// @brief Retrieves the SDL handle of the window.
    /// 
    /// This method returns the SDL-specific handle for the window, which is useful for SDL-based
    /// operations.
    /// 
    /// @return The SDL window handle.
    SDL_Window* GetSDLHandle() { return mWindow; }

private:
    bool mRunning = true; ///< Indicates whether the window is open and running.
    SDL_Window* mWindow = nullptr; ///< The SDL window handle.
};
