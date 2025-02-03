//
// > Notice: Amélie Heinrich @ 2024
// > Create Time: 2025-02-03 12:20:52
//

#include <Core/Application.hpp>
#include <Core/Logger.hpp>

#include <Input/Input.hpp>

Application::Application(ApplicationSpecs specs)
    : mApplicationSpecs(specs)
{
    Logger::Init();
    Input::Init();

    mWindow = MakeRef<Window>(specs.Width, specs.Height, specs.WindowTitle);

    LOG_INFO("Initialized {0} running Mnemen Engine", specs.GameName);
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

        Input::PostUpdate();
    }
}
