//
// > Notice: Amélie Heinrich @ 2025
// > Create Time: 2025-02-03 15:13:47
//

#include "Input.hpp"

#include <Core/Logger.hpp>
#include <Core/Profiler.hpp>

Input::InputData Input::sData;

void Input::Init()
{
    sData.Keys[SDLK_TAB].State = KeyState::Up;
    sData.Keys[SDLK_LEFT].State = KeyState::Up;
    sData.Keys[SDLK_RIGHT].State = KeyState::Up;
    sData.Keys[SDLK_UP].State = KeyState::Up;
    sData.Keys[SDLK_DOWN].State = KeyState::Up;
    sData.Keys[SDLK_PAGEUP].State = KeyState::Up;
    sData.Keys[SDLK_PAGEDOWN].State = KeyState::Up;
    sData.Keys[SDLK_HOME].State = KeyState::Up;
    sData.Keys[SDLK_END].State = KeyState::Up;
    sData.Keys[SDLK_INSERT].State = KeyState::Up;
    sData.Keys[SDLK_DELETE].State = KeyState::Up;
    sData.Keys[SDLK_BACKSPACE].State = KeyState::Up;
    sData.Keys[SDLK_SPACE].State = KeyState::Up;
    sData.Keys[SDLK_RETURN].State = KeyState::Up;
    sData.Keys[SDLK_ESCAPE].State = KeyState::Up;
    sData.Keys[SDLK_APOSTROPHE].State = KeyState::Up;
    sData.Keys[SDLK_COMMA].State = KeyState::Up;
    sData.Keys[SDLK_MINUS].State = KeyState::Up;
    sData.Keys[SDLK_PERIOD].State = KeyState::Up;
    sData.Keys[SDLK_SLASH].State = KeyState::Up;
    sData.Keys[SDLK_SEMICOLON].State = KeyState::Up;
    sData.Keys[SDLK_EQUALS].State = KeyState::Up;
    sData.Keys[SDLK_LEFTBRACKET].State = KeyState::Up;
    sData.Keys[SDLK_BACKSLASH].State = KeyState::Up;
    sData.Keys[SDLK_RIGHTBRACKET].State = KeyState::Up;
    sData.Keys[SDLK_GRAVE].State = KeyState::Up;
    sData.Keys[SDLK_CAPSLOCK].State = KeyState::Up;
    sData.Keys[SDLK_SCROLLLOCK].State = KeyState::Up;
    sData.Keys[SDLK_NUMLOCKCLEAR].State = KeyState::Up;
    sData.Keys[SDLK_PRINTSCREEN].State = KeyState::Up;
    sData.Keys[SDLK_PAUSE].State = KeyState::Up;
    sData.Keys[SDLK_LCTRL].State = KeyState::Up;
    sData.Keys[SDLK_LSHIFT].State = KeyState::Up;
    sData.Keys[SDLK_LALT].State = KeyState::Up;
    sData.Keys[SDLK_LGUI].State = KeyState::Up;
    sData.Keys[SDLK_RCTRL].State = KeyState::Up;
    sData.Keys[SDLK_RSHIFT].State = KeyState::Up;
    sData.Keys[SDLK_RALT].State = KeyState::Up;
    sData.Keys[SDLK_RGUI].State = KeyState::Up;
    sData.Keys[SDLK_APPLICATION].State = KeyState::Up;
    sData.Keys[SDLK_0].State = KeyState::Up;
    sData.Keys[SDLK_1].State = KeyState::Up;
    sData.Keys[SDLK_2].State = KeyState::Up;
    sData.Keys[SDLK_3].State = KeyState::Up;
    sData.Keys[SDLK_4].State = KeyState::Up;
    sData.Keys[SDLK_5].State = KeyState::Up;
    sData.Keys[SDLK_6].State = KeyState::Up;
    sData.Keys[SDLK_7].State = KeyState::Up;
    sData.Keys[SDLK_8].State = KeyState::Up;
    sData.Keys[SDLK_9].State = KeyState::Up;
    sData.Keys[SDLK_A].State = KeyState::Up;
    sData.Keys[SDLK_B].State = KeyState::Up;
    sData.Keys[SDLK_C].State = KeyState::Up;
    sData.Keys[SDLK_D].State = KeyState::Up;
    sData.Keys[SDLK_E].State = KeyState::Up;
    sData.Keys[SDLK_F].State = KeyState::Up;
    sData.Keys[SDLK_G].State = KeyState::Up;
    sData.Keys[SDLK_H].State = KeyState::Up;
    sData.Keys[SDLK_I].State = KeyState::Up;
    sData.Keys[SDLK_J].State = KeyState::Up;
    sData.Keys[SDLK_K].State = KeyState::Up;
    sData.Keys[SDLK_L].State = KeyState::Up;
    sData.Keys[SDLK_M].State = KeyState::Up;
    sData.Keys[SDLK_N].State = KeyState::Up;
    sData.Keys[SDLK_O].State = KeyState::Up;
    sData.Keys[SDLK_P].State = KeyState::Up;
    sData.Keys[SDLK_Q].State = KeyState::Up;
    sData.Keys[SDLK_R].State = KeyState::Up;
    sData.Keys[SDLK_S].State = KeyState::Up;
    sData.Keys[SDLK_T].State = KeyState::Up;
    sData.Keys[SDLK_U].State = KeyState::Up;
    sData.Keys[SDLK_V].State = KeyState::Up;
    sData.Keys[SDLK_W].State = KeyState::Up;
    sData.Keys[SDLK_X].State = KeyState::Up;
    sData.Keys[SDLK_Y].State = KeyState::Up;
    sData.Keys[SDLK_Z].State = KeyState::Up;
    sData.Keys[SDLK_F1].State = KeyState::Up;
    sData.Keys[SDLK_F2].State = KeyState::Up;
    sData.Keys[SDLK_F3].State = KeyState::Up;
    sData.Keys[SDLK_F4].State = KeyState::Up;
    sData.Keys[SDLK_F5].State = KeyState::Up;
    sData.Keys[SDLK_F6].State = KeyState::Up;
    sData.Keys[SDLK_F7].State = KeyState::Up;
    sData.Keys[SDLK_F8].State = KeyState::Up;
    sData.Keys[SDLK_F9].State = KeyState::Up;
    sData.Keys[SDLK_F10].State = KeyState::Up;
    sData.Keys[SDLK_F11].State = KeyState::Up;
    sData.Keys[SDLK_F12].State = KeyState::Up;
    sData.Keys[SDLK_F13].State = KeyState::Up;
    sData.Keys[SDLK_F14].State = KeyState::Up;
    sData.Keys[SDLK_F15].State = KeyState::Up;
    sData.Keys[SDLK_F16].State = KeyState::Up;
    sData.Keys[SDLK_F17].State = KeyState::Up;
    sData.Keys[SDLK_F18].State = KeyState::Up;
    sData.Keys[SDLK_F19].State = KeyState::Up;
    sData.Keys[SDLK_F20].State = KeyState::Up;
    sData.Keys[SDLK_F21].State = KeyState::Up;
    sData.Keys[SDLK_F22].State = KeyState::Up;
    sData.Keys[SDLK_F23].State = KeyState::Up;
    sData.Keys[SDLK_F24].State = KeyState::Up;
    sData.Keys[SDLK_AC_BACK].State = KeyState::Up;
    sData.Keys[SDLK_AC_FORWARD].State = KeyState::Up;

    sData.Buttons[SDL_BUTTON_LEFT] = false;
    sData.Buttons[SDL_BUTTON_RIGHT] = false;
    sData.Buttons[SDL_BUTTON_MIDDLE] = false;

    LOG_INFO("Initialized Input System");
}

void Input::Update(SDL_Event* event)
{
    UInt64 timestamp = SDL_GetTicks();

    sData.MousePos = GetMousePosition();

    switch (event->type) {
        case SDL_EVENT_KEY_DOWN: {
            SDL_Keycode key = event->key.key;
            if (event->key.repeat) {
                sData.Keys[key].State = KeyState::Held;
            } else {
                sData.Keys[key].State = KeyState::Pressed;
            }
            sData.Keys[key].TimeStamp = timestamp;
            break;
        };
        case SDL_EVENT_KEY_UP: {
            SDL_Keycode key = event->key.key;
            sData.Keys[key].State = KeyState::Up;
            sData.Keys[key].TimeStamp = timestamp;
            break;
        };
        case SDL_EVENT_MOUSE_BUTTON_DOWN: {
            sData.Buttons[event->button.button] = true;
            break;
        };
        case SDL_EVENT_MOUSE_BUTTON_UP: {
            sData.Buttons[event->button.button] = false;
            break;
        };
    }
}

void Input::PostUpdate()
{
    sData.MouseDelta = GetMousePosition() - sData.MousePos;

    for (auto& key : sData.Keys) {
        UInt64 timestamp = SDL_GetTicks();
        if (sData.Keys[key.first].State == KeyState::Pressed && sData.Keys[key.first].TimeStamp != timestamp)
            sData.Keys[key.first].State = KeyState::Held;
    }
}

void Input::Exit()
{

}

bool Input::IsKeyPressed(SDL_Keycode key)
{
    return sData.Keys[key].State == KeyState::Pressed;
}

bool Input::IsKeyHeld(SDL_Keycode key)
{
    return sData.Keys[key].State == KeyState::Held;
}

bool Input::IsKeyDown(SDL_Keycode key)
{
    return sData.Keys[key].State != KeyState::Up;
}

bool Input::IsKeyUp(SDL_Keycode key)
{
    return sData.Keys[key].State == KeyState::Up;
}

bool Input::IsButtonPressed(UInt8 button)
{
    return sData.Buttons[button] == true;
}

bool Input::IsButtonUp(UInt8 button)
{
    return sData.Buttons[button] == false;
}

glm::vec2 Input::GetMousePosition()
{
    Float32 x, y;
    SDL_GetGlobalMouseState(&x, &y);
    return glm::vec2(x, y);
}

glm::vec2 Input::GetMouseDelta()
{
    return sData.MouseDelta;
}
