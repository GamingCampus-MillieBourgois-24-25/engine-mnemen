//
// > Notice: Amélie Heinrich @ 2025
// > Create Time: 2025-02-13 18:38:50
//

#include "ScriptBinding.hpp"
#include "ScriptWrapper.hpp"

#include <World/Entity.hpp>
#include <Input/Input.hpp>

void ScriptBinding::InitBindings(sol::state& state)
{
    InitVec(state);
    InitQuat(state);
    InitTransform(state);
    InitKeycode(state);
    InitInput(state);
    InitEntity(state);
}

void ScriptBinding::InitEntity(sol::state& state)
{
    state["Entity"] = state.create_table();
    state["Entity"]["GetName"] = &LuaWrapper::LuaEntity::GetName;
    state["Entity"]["SetName"] = &LuaWrapper::LuaEntity::SetName;
    state["Entity"]["GetTransform"] = &LuaWrapper::LuaEntity::GetTransform;
}

void ScriptBinding::InitVec(sol::state& state)
{
    state.new_usertype<glm::vec2>(
        "vec2",
        sol::constructors<glm::vec2(), glm::vec2(float, float)>(),
        "x", &glm::vec2::x,
        "y", &glm::vec2::y
    );

    state.new_usertype<glm::vec3>(
        "vec3",
        sol::constructors<glm::vec3(), glm::vec3(float, float, float)>(),
        "x", &glm::vec3::x,
        "y", &glm::vec3::y,
        "z", &glm::vec3::z
    );

    state.new_usertype<glm::vec4>(
        "vec4",
        sol::constructors<glm::vec4(), glm::vec4(float, float, float, float)>(),
        "x", &glm::vec4::x,
        "y", &glm::vec4::y,
        "z", &glm::vec4::z,
        "w", &glm::vec4::w
    );
}

void ScriptBinding::InitQuat(sol::state& state)
{
    state.new_usertype<glm::quat>(
        "quat",
        sol::constructors<glm::quat(), glm::quat(float, const glm::vec3&)>(),
        "x", &glm::quat::x,
        "y", &glm::quat::y,
        "z", &glm::quat::z,
        "w", &glm::quat::w
    );
}

void ScriptBinding::InitInput(sol::state& state)
{
    state["KeyState"] = state.create_table_with(
        "Pressed", KeyState::Pressed,
        "Held", KeyState::Held,
        "Up", KeyState::Up
    );

    auto input = state.create_table("Input");
    input["Init"] = &Input::Init;
    input["Update"] = &Input::Update;
    input["PostUpdate"] = &Input::PostUpdate;
    input["Exit"] = &Input::Exit;
    input["IsKeyPressed"] = &Input::IsKeyPressed;
    input["IsKeyHeld"] = &Input::IsKeyHeld;
    input["IsKeyDown"] = &Input::IsKeyDown;
    input["IsKeyUp"] = &Input::IsKeyUp;
    input["IsButtonPressed"] = &Input::IsButtonPressed;
    input["IsButtonUp"] = &Input::IsButtonUp;
    input["GetMousePosition"] = &Input::GetMousePosition;
    input["GetMouseDelta"] = &Input::GetMouseDelta;
}

void ScriptBinding::InitTransform(sol::state& state)
{
    state.new_usertype<TransformComponent>(
        "TransformComponent",
        sol::constructors<TransformComponent()>(),
        "position", &TransformComponent::Position,
        "scale", &TransformComponent::Scale,
        "rotation", &TransformComponent::Rotation
    );
}

void ScriptBinding::InitKeycode(sol::state& state)
{
    state["Keycode"] = state.create_table_with("Tab", LuaWrapper::Keycode::Tab,
                                               "Left", LuaWrapper::Keycode::Left,
                                               "Right", LuaWrapper::Keycode::Right,
                                               "Up", LuaWrapper::Keycode::Up,
                                               "Down", LuaWrapper::Keycode::Down,
                                               "PageUp", LuaWrapper::Keycode::PageUp,
                                               "PageDown", LuaWrapper::Keycode::PageDown,
                                               "Home", LuaWrapper::Keycode::Home,
                                               "End", LuaWrapper::Keycode::End,
                                               "Insert", LuaWrapper::Keycode::Insert,
                                               "Delete", LuaWrapper::Keycode::Delete,
                                               "Backspace", LuaWrapper::Keycode::Backspace,
                                               "Space", LuaWrapper::Keycode::Space,
                                               "Return", LuaWrapper::Keycode::Return,
                                               "Escape", LuaWrapper::Keycode::Escape,
                                               "Apostrophe", LuaWrapper::Keycode::Apostrophe,
                                               "Comma", LuaWrapper::Keycode::Comma,
                                               "Minus", LuaWrapper::Keycode::Minus,
                                               "Period", LuaWrapper::Keycode::Period,
                                               "Slash", LuaWrapper::Keycode::Slash,
                                               "Semicolon", LuaWrapper::Keycode::Semicolon,
                                               "Equals", LuaWrapper::Keycode::Equals,
                                               "LeftBracket", LuaWrapper::Keycode::LeftBracket,
                                               "Backslash", LuaWrapper::Keycode::Backslash,
                                               "RightBracket", LuaWrapper::Keycode::RightBracket,
                                               "Grave", LuaWrapper::Keycode::Grave,
                                               "CapsLock", LuaWrapper::Keycode::CapsLock,
                                               "ScrollLock", LuaWrapper::Keycode::ScrollLock,
                                               "NumLock", LuaWrapper::Keycode::NumLock,
                                               "PrintScreen", LuaWrapper::Keycode::PrintScreen,
                                               "Pause", LuaWrapper::Keycode::Pause,
                                               "LCtrl", LuaWrapper::Keycode::LCtrl,
                                               "LShift", LuaWrapper::Keycode::LShift,
                                               "LAlt", LuaWrapper::Keycode::LAlt,
                                               "LGUI", LuaWrapper::Keycode::LGUI,
                                               "RCtrl", LuaWrapper::Keycode::RCtrl,
                                               "RShift", LuaWrapper::Keycode::RShift,
                                               "RAlt", LuaWrapper::Keycode::RAlt,
                                               "RGUI", LuaWrapper::Keycode::RGUI,
                                               "Application", LuaWrapper::Keycode::Application,
                                               "Key0", LuaWrapper::Keycode::Key0,
                                               "Key1", LuaWrapper::Keycode::Key1,
                                               "Key2", LuaWrapper::Keycode::Key2,
                                               "Key3", LuaWrapper::Keycode::Key3,
                                               "Key4", LuaWrapper::Keycode::Key4,
                                               "Key5", LuaWrapper::Keycode::Key5,
                                               "Key6", LuaWrapper::Keycode::Key6,
                                               "Key7", LuaWrapper::Keycode::Key7,
                                               "Key8", LuaWrapper::Keycode::Key8,
                                               "Key9", LuaWrapper::Keycode::Key9,
                                               "A", LuaWrapper::Keycode::A,
                                               "B", LuaWrapper::Keycode::B,
                                               "C", LuaWrapper::Keycode::C,
                                               "D", LuaWrapper::Keycode::D,
                                               "E", LuaWrapper::Keycode::E,
                                               "F", LuaWrapper::Keycode::F,
                                               "G", LuaWrapper::Keycode::G,
                                               "H", LuaWrapper::Keycode::H,
                                               "I", LuaWrapper::Keycode::I,
                                               "J", LuaWrapper::Keycode::J,
                                               "K", LuaWrapper::Keycode::K,
                                               "L", LuaWrapper::Keycode::L,
                                               "M", LuaWrapper::Keycode::M,
                                               "N", LuaWrapper::Keycode::N,
                                               "O", LuaWrapper::Keycode::O,
                                               "P", LuaWrapper::Keycode::P,
                                               "Q", LuaWrapper::Keycode::Q,
                                               "R", LuaWrapper::Keycode::R,
                                               "S", LuaWrapper::Keycode::S,
                                               "T", LuaWrapper::Keycode::T,
                                               "U", LuaWrapper::Keycode::U,
                                               "V", LuaWrapper::Keycode::V,
                                               "W", LuaWrapper::Keycode::W,
                                               "X", LuaWrapper::Keycode::X,
                                               "Y", LuaWrapper::Keycode::Y,
                                               "Z", LuaWrapper::Keycode::Z,
                                               "F1", LuaWrapper::Keycode::F1,
                                               "F2", LuaWrapper::Keycode::F2,
                                               "F3", LuaWrapper::Keycode::F3,
                                               "F4", LuaWrapper::Keycode::F4,
                                               "F5", LuaWrapper::Keycode::F5,
                                               "F6", LuaWrapper::Keycode::F6,
                                               "F7", LuaWrapper::Keycode::F7,
                                               "F8", LuaWrapper::Keycode::F8,
                                               "F9", LuaWrapper::Keycode::F9,
                                               "F10", LuaWrapper::Keycode::F10,
                                               "F11", LuaWrapper::Keycode::F11,
                                               "F12", LuaWrapper::Keycode::F12,
                                               "F13", LuaWrapper::Keycode::F13,
                                               "F14", LuaWrapper::Keycode::F14,
                                               "F15", LuaWrapper::Keycode::F15,
                                               "F16", LuaWrapper::Keycode::F16,
                                               "F17", LuaWrapper::Keycode::F17,
                                               "F18", LuaWrapper::Keycode::F18,
                                               "F19", LuaWrapper::Keycode::F19,
                                               "F20", LuaWrapper::Keycode::F20,
                                               "F21", LuaWrapper::Keycode::F21,
                                               "F22", LuaWrapper::Keycode::F22,
                                               "F23", LuaWrapper::Keycode::F23,
                                               "F24", LuaWrapper::Keycode::F24,
                                               "AC_Back", LuaWrapper::Keycode::AC_Back,
                                               "AC_Forward", LuaWrapper::Keycode::AC_Forward);
}
