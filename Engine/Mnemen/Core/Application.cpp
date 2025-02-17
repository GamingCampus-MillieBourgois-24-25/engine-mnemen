//
// > Notice: Amélie Heinrich @ 2025
// > Create Time: 2025-02-03 12:20:52
//

#include <Core/Application.hpp>
#include <Core/Logger.hpp>
#include <Core/Profiler.hpp>
#include <Core/Assert.hpp>

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
    AudioSystem::Init();
    AISystem::Init();
    ScriptSystem::Init();

    mWindow = MakeRef<Window>(specs.Width, specs.Height, specs.WindowTitle);
    mRHI = MakeRef<RHI>(mWindow);

    mProject = MakeRef<Project>();
    if (!specs.ProjectPath.empty())
        mProject->Load(specs.ProjectPath);

    Profiler::Init(mRHI);
    AssetManager::Init(mRHI);
    AssetCacher::Init("Assets");

    mRenderer = MakeRef<Renderer>(mRHI);
    if (!mProject->StartScenePathRelative.empty())
        mScene = SceneSerializer::DeserializeScene(mProject->StartScenePathRelative);
    else
        mScene = MakeRef<Scene>();
       
    LOG_INFO("Initialized Mnemen! Ready to rock 8)");
}

Application::~Application()
{
    AssetManager::Purge();
    Profiler::Exit();
    ScriptSystem::Exit();
    AISystem::Exit();
    AudioSystem::Exit();
    PhysicsSystem::Exit();
    Input::Exit();

    LOG_INFO("Mnemen is done!");
}

void Application::OnAwake()
{
    mScenePlaying = true;

    ScriptSystem::Awake(mScene);
    AudioSystem::Awake(mScene);
}

void Application::OnStop()
{
    AudioSystem::Quit(mScene);
    ScriptSystem::Quit(mScene);

    mScenePlaying = false;
}

void Application::Run()
{
    Uploader::Flush();
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
            float minStepDuration = 1.0f / mProject->Settings.PhysicsRefreshRate;
            if (TO_SECONDS(mPhysicsTimer.GetElapsed()) > minStepDuration) {
                OnPhysicsTick();
                if (mScenePlaying && mScene) {
                    PhysicsSystem::Update(mScene, minStepDuration);
                }
                mPhysicsTimer.Restart();
            }
        }

        // Engine Update
        {
            PROFILE_SCOPE("Systems Update");
            
            mWindow->Update();
            AssetManager::Update();
            if (mScenePlaying && mScene) {
                AISystem::Update(mScene);
                AudioSystem::Update(mScene);
                ScriptSystem::Update(mScene, dt);
            }
            if (mScene)
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
    mRHI->Wait();
    AssetManager::Clean();
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
    PostPresent();
}
