//
// > Notice: Amélie Heinrich @ 2025
// > Create Time: 2025-02-03 12:20:52
//

#include <Core/Application.hpp>
#include <Core/Logger.hpp>

#include <Input/Input.hpp>
#include <Asset/AssetCacher.hpp>
#include <Asset/AssetManager.hpp>

#include <RHI/Uploader.hpp>
#include <Renderer/RendererTools.hpp>

#include <Physics/PhysicsSystem.hpp>
#include <Audio/AudioSystem.hpp>
#include <AI/AISystem.hpp>
#include <Script/ScriptSystem.hpp>

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

    AssetManager::Init(mRHI);
    AssetCacher::Init("Assets");

    mRenderer = MakeRef<Renderer>(mRHI);

    LOG_INFO("Initialized Mnemen! Ready to rock 8)");
}

Application::~Application()
{
    ScriptSystem::Exit();
    AISystem::Exit();
    AudioSystem::Exit();
    PhysicsSystem::Exit();
    Input::Exit();
}

void Application::Run()
{
    Uploader::Flush();
    mRHI->Wait();
    while (mWindow->IsOpen()) {
        float time = mTimer.GetElapsed();
        float dt = time - mLastFrame;
        mLastFrame = time;
        dt /= 1000.0f;

        // On Physics Update
        {
            float minStepDuration = 1.0f / mApplicationSpecs.PhysicsRefreshRate;
            if (TO_SECONDS(mPhysicsTimer.GetElapsed()) > minStepDuration) {
                OnPhysicsTick();
                PhysicsSystem::Update(mScene, minStepDuration);
                mPhysicsTimer.Restart();
            }
        }

        // Update
        {
            AISystem::Update(mScene);
            AudioSystem::Update(mScene);
            ScriptSystem::Update(mScene);
            mWindow->Update();
            mScene.Update();
            OnUpdate(dt);
        }
        
        // Render
        {
            OnPrivateRender();
        }
        Input::PostUpdate();
    }
    mRHI->Wait();
}

void Application::OnPrivateRender()
{
    Frame frame = mRHI->Begin();
    frame.CommandBuffer->Begin();

    // Scene render
    {
        mRenderer->Render(frame, mScene);
    }

    // UI
    {
        frame.CommandBuffer->BeginMarker("ImGui");
        frame.CommandBuffer->Barrier(frame.Backbuffer, ResourceLayout::ColorWrite);
        frame.CommandBuffer->SetRenderTargets({ frame.BackbufferView }, nullptr);
        frame.CommandBuffer->BeginGUI(frame.Width, frame.Height);
        OnImGui();
        frame.CommandBuffer->EndGUI();
        frame.CommandBuffer->Barrier(frame.Backbuffer, ResourceLayout::Present);
        frame.CommandBuffer->EndMarker();
    }

    frame.CommandBuffer->End();
    mRHI->Submit({ frame.CommandBuffer });
    mRHI->End();
    mRHI->Present(false);
}
