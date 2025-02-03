//
// > Notice: Amélie Heinrich @ 2024
// > Create Time: 2025-02-03 12:51:54
//

#pragma once

#include <SDL3/SDL.h>
#include "Common.hpp"

class Window
{
public:
    /// @brief Creates a window
    /// @param width The width of the window
    /// @param height The height of the window
    /// @param title The title of the window
    Window(int width, int height, const String& title);
    ~Window();

    /// @brief Returns if the window is open
    /// @return If the window is open, true, otherwise, false
    bool IsOpen() const { return mRunning; }

    /// @brief Updates the window
    void Update();
private:
    bool mRunning = true;
    SDL_Window* mWindow = nullptr;
};
