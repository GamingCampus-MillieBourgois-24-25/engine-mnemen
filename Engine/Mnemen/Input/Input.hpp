//
// > Notice: Amélie Heinrich @ 2024
// > Create Time: 2025-02-03 14:48:27
//

#pragma once

#include <SDL3/SDL.h>
#include <Core/Common.hpp>

enum class KeyState
{
    Pressed,
    Held,
    Up
};

class Input
{
public:
    /// @brief Initializes the input system
    static void Init();

    /// @brief Updates the input system
    static void Update(SDL_Event* event);
    
    /// @brief Sets some flags that the input system will use next frame
    static void PostUpdate();

    /// @brief Exist the input system
    static void Exit();

    /// @brief Returns whether the current key is pressed
    /// @param key The key
    /// @return True if pressed, false otherwise
    static bool IsKeyPressed(SDL_Keycode key);

    /// @brief Returns whether the current key is held
    /// @param key The key
    /// @return True if held, false otherwise
    static bool IsKeyHeld(SDL_Keycode key);
    
    /// @brief Returns whether the current key is pressed or held
    /// @param key The key
    /// @return True if pressed or held, false otherwise
    static bool IsKeyDown(SDL_Keycode key);

    /// @brief Returns whether the current key is up
    /// @param key The key
    /// @return True if up, false otherwise
    static bool IsKeyUp(SDL_Keycode key);
private:
    struct KeyInfo {
        KeyState State;
        UInt64 TimeStamp;
    };
    
    struct InputData
    {
        UnorderedMap<SDL_Keycode, KeyInfo> Keys;
    };
    static InputData sData;
};
