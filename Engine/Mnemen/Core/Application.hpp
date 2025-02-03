//
// > Notice: Amélie Heinrich @ 2024
// > Create Time: 2025-02-03 12:17:26
//

#pragma once

#include "Common.hpp"
#include "Window.hpp"
#include "../RHI/RHI.hpp"

/// @brief Structure that contains the settings of the application
struct ApplicationSpecs
{
    /// @brief The width of the application window
    int Width;
    /// @brief The height of the application window
    int Height;

    /// @brief The name of the game
    String GameName;
    /// @brief The title of the game window
    String WindowTitle;
};

/// @brief Uniform class used to describe an application using the engine
class Application
{
public:
    Application(ApplicationSpecs specs);
    ~Application();

    /// @brief Called once every frame
    virtual void OnUpdate() = 0;
    /// @brief Called once every physics update, 90hz
    virtual void OnPhysicsTick() = 0;
    /// @brief Called between begin/end frame for ImGui
    virtual void OnImGui() = 0;

    /// @brief Runs the application
    void Run();
protected:
    void OnPrivateRender();

    /// @brief a copy of the application settings
    ApplicationSpecs mApplicationSpecs;

    /// @brief The application window
    Ref<Window> mWindow;

    /// @brief The RHI
    RHI::Ref mRHI;
};
