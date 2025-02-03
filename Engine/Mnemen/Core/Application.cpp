//
// > Notice: Amélie Heinrich @ 2024
// > Create Time: 2025-02-03 12:20:52
//

#include <Core/Application.hpp>
#include <Core/Logger.hpp>

#include <Input/Input.hpp>
#include <Asset/AssetCacher.hpp>
#include <Asset/AssetManager.hpp>

Application::Application(ApplicationSpecs specs)
    : mApplicationSpecs(specs)
{
    Logger::Init();
    Input::Init();

    mWindow = MakeRef<Window>(specs.Width, specs.Height, specs.WindowTitle);
    mRHI = MakeRef<RHI>(mWindow);
    mRenderer = MakeRef<Renderer>(mRHI);

    AssetManager::Init(mRHI);
    AssetCacher::Init("Assets");

    LOG_INFO("Initialized {0} running Mnemen Engine", specs.GameName);

    mRHI->Wait();
}

Application::~Application()
{
    Input::Exit();
}

void Application::Run()
{
    while (mWindow->IsOpen()) {
        mWindow->Update();
        OnUpdate();
        OnPrivateRender();
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
        mRenderer->Render(frame);
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
