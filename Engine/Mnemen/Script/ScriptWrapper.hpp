//
// > Notice: Amélie Heinrich @ 2025
// > Create Time: 2025-02-13 18:39:56
//

#pragma once

#include <Core/Common.hpp>

#include <World/Entity.hpp>

namespace LuaWrapper
{
    enum class Keycode
    {
        Tab = SDLK_TAB,
        Left = SDLK_LEFT,
        Right = SDLK_RIGHT,
        Up = SDLK_UP,
        Down = SDLK_DOWN,
        PageUp = SDLK_PAGEUP,
        PageDown = SDLK_PAGEDOWN,
        Home = SDLK_HOME,
        End = SDLK_END,
        Insert = SDLK_INSERT,
        Delete = SDLK_DELETE,
        Backspace = SDLK_BACKSPACE,
        Space = SDLK_SPACE,
        Return = SDLK_RETURN,
        Escape = SDLK_ESCAPE,
        Apostrophe = SDLK_APOSTROPHE,
        Comma = SDLK_COMMA,
        Minus = SDLK_MINUS,
        Period = SDLK_PERIOD,
        Slash = SDLK_SLASH,
        Semicolon = SDLK_SEMICOLON,
        Equals = SDLK_EQUALS,
        LeftBracket = SDLK_LEFTBRACKET,
        Backslash = SDLK_BACKSLASH,
        RightBracket = SDLK_RIGHTBRACKET,
        Grave = SDLK_GRAVE,
        CapsLock = SDLK_CAPSLOCK,
        ScrollLock = SDLK_SCROLLLOCK,
        NumLock = SDLK_NUMLOCKCLEAR,
        PrintScreen = SDLK_PRINTSCREEN,
        Pause = SDLK_PAUSE,
        LCtrl = SDLK_LCTRL,
        LShift = SDLK_LSHIFT,
        LAlt = SDLK_LALT,
        LGUI = SDLK_LGUI,
        RCtrl = SDLK_RCTRL,
        RShift = SDLK_RSHIFT,
        RAlt = SDLK_RALT,
        RGUI = SDLK_RGUI,
        Application = SDLK_APPLICATION,
        Key0 = SDLK_0,
        Key1 = SDLK_1,
        Key2 = SDLK_2,
        Key3 = SDLK_3,
        Key4 = SDLK_4,
        Key5 = SDLK_5,
        Key6 = SDLK_6,
        Key7 = SDLK_7,
        Key8 = SDLK_8,
        Key9 = SDLK_9,
        A = SDLK_A,
        B = SDLK_B,
        C = SDLK_C,
        D = SDLK_D,
        E = SDLK_E,
        F = SDLK_F,
        G = SDLK_G,
        H = SDLK_H,
        I = SDLK_I,
        J = SDLK_J,
        K = SDLK_K,
        L = SDLK_L,
        M = SDLK_M,
        N = SDLK_N,
        O = SDLK_O,
        P = SDLK_P,
        Q = SDLK_Q,
        R = SDLK_R,
        S = SDLK_S,
        T = SDLK_T,
        U = SDLK_U,
        V = SDLK_V,
        W = SDLK_W,
        X = SDLK_X,
        Y = SDLK_Y,
        Z = SDLK_Z,
        F1 = SDLK_F1,
        F2 = SDLK_F2,
        F3 = SDLK_F3,
        F4 = SDLK_F4,
        F5 = SDLK_F5,
        F6 = SDLK_F6,
        F7 = SDLK_F7,
        F8 = SDLK_F8,
        F9 = SDLK_F9,
        F10 = SDLK_F10,
        F11 = SDLK_F11,
        F12 = SDLK_F12,
        F13 = SDLK_F13,
        F14 = SDLK_F14,
        F15 = SDLK_F15,
        F16 = SDLK_F16,
        F17 = SDLK_F17,
        F18 = SDLK_F18,
        F19 = SDLK_F19,
        F20 = SDLK_F20,
        F21 = SDLK_F21,
        F22 = SDLK_F22,
        F23 = SDLK_F23,
        F24 = SDLK_F24,
        AC_Back = SDLK_AC_BACK,
        AC_Forward = SDLK_AC_FORWARD
    };

    class LuaEntity
    {
    public:
        // TODO: delete

        static String GetName(int entity);
        static void SetName(int entity, const char* name);

        static TransformComponent& GetTransform(int entity);
    };
}
