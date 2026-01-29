#include "Engine/WindowManager.hpp"
#include <iostream>

namespace Engine
{

    WindowManager::WindowManager(const std::string &title, int width, int height)
    {
        if (!SDL_Init(SDL_INIT_VIDEO))
        {
            std::cerr << "SDL_Init Error: " << SDL_GetError() << std::endl;
            m_running = false;
            return;
        }

        SDL_Window *window = nullptr;
        SDL_Renderer *renderer = nullptr;

        // SDL3 combined call to create both window and hardware renderer
        if (!SDL_CreateWindowAndRenderer(title.c_str(), width, height, 0, &window, &renderer))
        {
            std::cerr << "SDL_Create Error: " << SDL_GetError() << std::endl;
            m_running = false;
            return;
        }

        m_window.reset(window);
        m_renderer.reset(renderer);
    }

    WindowManager::~WindowManager()
    {
        SDL_Quit();
    }

    void WindowManager::pollEvents()
    {
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_EVENT_QUIT)
            {
                m_running = false;
            }
        }
    }

    void WindowManager::render()
    {
        // Set color to Red (R: 255, G: 0, B: 0, A: 255)
        SDL_SetRenderDrawColor(m_renderer.get(), 255, 0, 0, 255);
        SDL_RenderClear(m_renderer.get());
        SDL_RenderPresent(m_renderer.get());
    }

} // namespace Engine