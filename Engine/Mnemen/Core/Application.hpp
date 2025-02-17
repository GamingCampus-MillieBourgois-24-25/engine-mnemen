//
// > Notice: Amélie Heinrich @ 2025
// > Create Time: 2025-02-03 12:17:26
//

#pragma once

#include "Common.hpp"
#include "Window.hpp"
#include "Timer.hpp"
#include "Project.hpp"

#include <RHI/RHI.hpp>
#include <Renderer/Renderer.hpp>
#include <World/Scene.hpp>

/// @struct ApplicationSpecs
/// @brief Stores configuration settings for the application.
struct ApplicationSpecs
{
    int Width; ///< Width of the application window in pixels.
    int Height; ///< Height of the application window in pixels.

    String WindowTitle; ///< Title displayed on the game window.
    String ProjectPath; ///< The path of the project.

    bool CopyToBackBuffer; ///< If set to true, the output color will be copied to the swapchain. Used in Runtime.
};

/// @class Application
/// @brief Base class representing an application using the engine.
///
/// This class provides the core framework for an application, including
/// update cycles, rendering, and physics processing.
class Application
{
public:
    /// @brief Constructs an application with the given specifications.
    /// @param specs The configuration settings for the application.
    Application(ApplicationSpecs specs);
    
    /// @brief Destroys the application and cleans up resources.
    ~Application();

    /// @brief Called once per frame to update application logic.
    /// @param dt Delta time since the last frame in seconds.
    virtual void OnUpdate(float dt) = 0;

    /// @brief Called at a fixed time step for physics updates (90 Hz by default).
    virtual void OnPhysicsTick() = 0;

    /// @brief Called during the UI rendering phase to handle ImGui drawing.
    virtual void OnImGui(const Frame& frame) = 0;

    /// @brief Called after present
    virtual void PostPresent() {};

    /// @brief Called when the scene is awaken
    void OnAwake();

    /// @brief Called when the scene is stopped
    void OnStop();

    /// @brief Starts the application loop.
    void Run();

    /// @brief Retrieves the main application window.
    /// @return A shared pointer to the window instance.
    Ref<Window> GetWindow() { return mWindow; }

    /// @brief Retrieves the current scene
    /// @return The active scene that the application is manipulating
    Ref<Scene> GetScene() { return mScene; }

    /// @brief Returns the loaded project
    /// @return The project that the application is running
    Ref<Project> GetProject() { return mProject; }
    
    /// @brief Returns the settings of the application.
    /// @return The settings of the application.
    ApplicationSpecs GetSpecs() { return mApplicationSpecs; }

    /// @brief Retrieves the current application instance.
    /// @return Pointer to the singleton application instance.
    static Application* Get() { return sInstance; }
protected:
    /// @brief Handles internal rendering operations.
    void OnPrivateRender();

    static Application* sInstance; ///< Singleton instance of the application.
    
    ApplicationSpecs mApplicationSpecs; ///< Cached application settings.
    Ref<Window> mWindow = nullptr; ///< Main application window.

    Timer mTimer; ///< Delta-time tracking timer.
    float mLastFrame = 0.0f; ///< Time of the last frame update.

    Timer mPhysicsTimer; ///< Timer for fixed-step physics updates.

    RHI::Ref mRHI = nullptr; ///< Rendering Hardware Interface.
    Renderer::Ref mRenderer = nullptr; ///< Renderer instance.
    
    Ref<Project> mProject = nullptr; ///< Currently active project.
    Ref<Scene> mScene = nullptr; ///< Currently active scene.

    bool mUIFocused = true; ///< Whether or not UI elements are focused.
    bool mScenePlaying = false; ///< Whether the scene is playing or not.
};

