//
// > Notice: Amélie Heinrich @ 2024
// > Create Time: 2025-02-03 12:53:32
//

#include "Window.hpp"
#include "Logger.hpp"

#include <Input/Input.hpp>

Window::Window(int width, int height, const String& title)
{
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        LOG_CRITICAL("Failed to initialize SDL3!");
    }

    mWindow = SDL_CreateWindow(title.c_str(), width, height, 0);
    if (!mWindow) {
        LOG_CRITICAL("Failed to create window!");
    }

    LOG_INFO("Created window {0} ({1}, {2})", title, width, height);
}

Window::~Window()
{
    SDL_DestroyWindow(mWindow);
    SDL_Quit();
}

void Window::Update()
{
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_EVENT_QUIT) {
            mRunning = false;
        }
        Input::Update(&event);
    }
}
