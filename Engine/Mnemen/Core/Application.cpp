//
// > Notice: Amélie Heinrich @ 2025
// > Create Time: 2025-02-03 12:20:52
//

#include <Core/Application.hpp>
#include <Core/Logger.hpp>
#include <Core/Profiler.hpp>

#include <Input/Input.hpp>
#include <Asset/AssetCacher.hpp>
#include <Asset/AssetManager.hpp>

#include <World/SceneSerializer.hpp>

#include <RHI/Uploader.hpp>
#include <Renderer/RendererTools.hpp>

#include <Physics/PhysicsSystem.hpp>
#include <Audio/AudioSystem.hpp>
#include <AI/AISystem.hpp>
#include <Script/ScriptSystem.hpp>

#include <imgui.h>

Application* Application::sInstance;

Application::Application(ApplicationSpecs specs)
    : mApplicationSpecs(specs)
{
    sInstance = this;

    Logger::Init();
    Input::Init();
    PhysicsSystem::Init();
    AudioSystem::Init("Assets/Audio/Back_music.mp3");
    AudioSystem::SetVolume(5.0f);
    AISystem::Init();
    ScriptSystem::Init();

    mWindow = MakeRef<Window>(specs.Width, specs.Height, specs.WindowTitle);
    mRHI = MakeRef<RHI>(mWindow);

    Profiler::Init(mRHI);
    AssetManager::Init(mRHI);
    AssetCacher::Init("Assets");

    mRenderer = MakeRef<Renderer>(mRHI);
    if (specs.StartScene.empty()) {
        mScene = MakeRef<Scene>();
    } else {
        mScene = SceneSerializer::DeserializeScene(specs.StartScene);
    }

    LOG_INFO("Initialized Mnemen! Ready to rock 8)");
}

Application::~Application()
{
    Profiler::Exit();
    ScriptSystem::Exit();
    AISystem::Exit();
    AudioSystem::Exit();
    PhysicsSystem::Exit();
    Input::Exit();
}

void Application::OnAwake()
{
    mScenePlaying = true;
    ScriptSystem::Awake(mScene);
}

void Application::OnStop()
{
    ScriptSystem::Quit(mScene);
    mScenePlaying = false;
}

void Application::Run()
{
    // Flush any uploads before running
    Uploader::Flush();
    mRHI->Wait();

    while (mWindow->IsOpen()) {
        Profiler::BeginFrame();

        PROFILE_SCOPE("App Run");
        float time = mTimer.GetElapsed();
        float dt = time - mLastFrame;
        mLastFrame = time;
        dt /= 1000.0f;

        // On Physics Update
        {
            PROFILE_SCOPE("Physics Update");
            float minStepDuration = 1.0f / mApplicationSpecs.PhysicsRefreshRate;
            if (TO_SECONDS(mPhysicsTimer.GetElapsed()) > minStepDuration) {
                OnPhysicsTick();
                if (mScenePlaying) {
                    PhysicsSystem::Update(mScene, minStepDuration);
                }
                mPhysicsTimer.Restart();
            }
        }

        // Engine Update
        {
            PROFILE_SCOPE("Systems Update");
            
            mWindow->Update();
            if (mScenePlaying) {
                AISystem::Update(mScene);
                AudioSystem::Update(mScene);
                ScriptSystem::Update(mScene, dt);
            }
            mScene->Update();
        }

        // App Update
        {
            PROFILE_SCOPE("App Update");
            OnUpdate(dt);
        }
        
        // Render
        {
            PROFILE_SCOPE("CPU Render");
            OnPrivateRender();
        }
        Input::PostUpdate();
    }
    ScriptSystem::Quit(mScene);
    mRHI->Wait();
}

void Application::OnPrivateRender()
{
    Frame frame = mRHI->Begin();
    frame.CommandBuffer->Begin();

    // PROFILE_SCOPE_GPU("Main Frame", frame.CommandBuffer);

    // Scene render
    {
        mRenderer->Render(frame, mScene);
    }

    // UI
    {
        // PROFILE_SCOPE_GPU("ImGui", frame.CommandBuffer);
        frame.CommandBuffer->BeginMarker("ImGui");
        frame.CommandBuffer->Barrier(frame.Backbuffer, ResourceLayout::ColorWrite);
        frame.CommandBuffer->SetRenderTargets({ frame.BackbufferView }, nullptr);
        frame.CommandBuffer->BeginGUI(frame.Width, frame.Height);
        
        OnImGui(frame);

        ImGuiIO& io = ImGui::GetIO();
        mUIFocused = io.WantCaptureMouse || io.WantCaptureKeyboard;

        frame.CommandBuffer->EndGUI();
        frame.CommandBuffer->Barrier(frame.Backbuffer, ResourceLayout::Present);
        frame.CommandBuffer->EndMarker();
    }

    // Profiler::ResolveGPUQueries(frame.CommandBuffer);
    frame.CommandBuffer->End();
    mRHI->Submit({ frame.CommandBuffer });
    mRHI->End();
    // Profiler::ReadbackGPUResults();

    mRHI->Present(false);
}
