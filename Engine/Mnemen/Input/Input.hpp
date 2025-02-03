//
// > Notice: Amélie Heinrich @ 2024
// > Create Time: 2025-02-03 14:48:27
//

#pragma once

#include <SDL3/SDL.h>
#include <glm/glm.hpp>
#include "../Core/Common.hpp"

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

    /// @brief Returns whether the current button is pressed
    /// @param button The button
    /// @return True if pressed, false otherwise
    static bool IsButtonPressed(UInt8 button);

    /// @brief Returns whether the current button is up
    /// @param button The button
    /// @return True if up, false otherwise
    static bool IsButtonUp(UInt8 button);

    /// @brief Returns the mouse position
    static glm::vec2 GetMousePosition();

    /// @brief Returns the mouse delta (mouse position - last mouse position)
    static glm::vec2 GetMouseDelta();
private:
    struct KeyInfo {
        KeyState State;
        UInt64 TimeStamp;
    };
    
    struct InputData
    {
        glm::vec2 MousePos;
        glm::vec2 MouseDelta;

        UnorderedMap<UInt8, bool> Buttons;
        UnorderedMap<SDL_Keycode, KeyInfo> Keys;
    };
    static InputData sData;
};
