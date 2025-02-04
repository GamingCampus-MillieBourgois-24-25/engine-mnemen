//
// > Notice: Amélie Heinrich @ 2025
// > Create Time: 2025-02-03 14:48:27
//

#pragma once

#include <SDL3/SDL.h>
#include <glm/glm.hpp>
#include "../Core/Common.hpp"

/// @brief The state of a key during input processing.
/// 
/// This enum defines the possible states of a key during input handling:
/// - Pressed: The key is currently being pressed.
/// - Held: The key is being held down.
/// - Up: The key has been released.
enum class KeyState
{
    Pressed, ///< The key is currently being pressed.
    Held,    ///< The key is being held down.
    Up       ///< The key has been released.
};

/// @brief A system for handling input events from the user.
/// 
/// The `Input` class provides static methods for initializing, updating, and querying the 
/// state of keyboard and mouse input. It allows you to check the current state of keys, 
/// buttons, and the mouse position.
class Input
{
public:
    /// @brief Initializes the input system.
    /// 
    /// This method sets up the input system, including any resources and event listeners 
    /// necessary to process input events.
    static void Init();

    /// @brief Updates the input system based on the given SDL event.
    /// 
    /// This method processes the current input event, updating the internal state of 
    /// the system. It is called every frame to process new events.
    /// 
    /// @param event The SDL event to process.
    static void Update(SDL_Event* event);

    /// @brief Sets flags that the input system will use in the next frame.
    /// 
    /// This method prepares the input system for the next frame, setting any flags or 
    /// internal state that will be used during the next update.
    static void PostUpdate();

    /// @brief Exits the input system and releases resources.
    /// 
    /// This method cleans up any resources used by the input system and prepares it for 
    /// shutdown.
    static void Exit();

    /// @brief Returns whether the specified key is pressed.
    /// 
    /// This method checks if the given key is currently pressed down.
    /// 
    /// @param key The SDL keycode of the key to check.
    /// @return `true` if the key is pressed, `false` otherwise.
    static bool IsKeyPressed(SDL_Keycode key);

    /// @brief Returns whether the specified key is held.
    /// 
    /// This method checks if the given key is currently being held down.
    /// 
    /// @param key The SDL keycode of the key to check.
    /// @return `true` if the key is held, `false` otherwise.
    static bool IsKeyHeld(SDL_Keycode key);
    
    /// @brief Returns whether the specified key is pressed or held.
    /// 
    /// This method checks if the given key is either pressed or held.
    /// 
    /// @param key The SDL keycode of the key to check.
    /// @return `true` if the key is pressed or held, `false` otherwise.
    static bool IsKeyDown(SDL_Keycode key);

    /// @brief Returns whether the specified key is released.
    /// 
    /// This method checks if the given key is released (up).
    /// 
    /// @param key The SDL keycode of the key to check.
    /// @return `true` if the key is released, `false` otherwise.
    static bool IsKeyUp(SDL_Keycode key);

    /// @brief Returns whether the specified mouse button is pressed.
    /// 
    /// This method checks if the given mouse button is currently pressed down.
    /// 
    /// @param button The mouse button to check.
    /// @return `true` if the button is pressed, `false` otherwise.
    static bool IsButtonPressed(UInt8 button);

    /// @brief Returns whether the specified mouse button is released.
    /// 
    /// This method checks if the given mouse button is released (up).
    /// 
    /// @param button The mouse button to check.
    /// @return `true` if the button is released, `false` otherwise.
    static bool IsButtonUp(UInt8 button);

    /// @brief Returns the current mouse position.
    /// 
    /// This method retrieves the current position of the mouse relative to the window.
    /// 
    /// @return The current mouse position as a `glm::vec2`.
    static glm::vec2 GetMousePosition();

    /// @brief Returns the mouse delta (change in mouse position).
    /// 
    /// This method retrieves the change in mouse position from the last frame.
    /// 
    /// @return The mouse delta as a `glm::vec2`.
    static glm::vec2 GetMouseDelta();

private:
    /// @brief Holds the state information for a single key.
    struct KeyInfo {
        KeyState State; ///< The current state of the key (pressed, held, or up).
        UInt64 TimeStamp; ///< The timestamp when the key state was last updated.
    };

    /// @brief Holds input data for the current frame.
    struct InputData
    {
        glm::vec2 MousePos; ///< The current mouse position.
        glm::vec2 MouseDelta; ///< The change in mouse position since the last frame.

        UnorderedMap<UInt8, bool> Buttons; ///< The state of the mouse buttons.
        UnorderedMap<SDL_Keycode, KeyInfo> Keys; ///< The state of the keys.
    };

    /// @brief Static instance of the input data.
    static InputData sData;
};

