#include "Engine/WindowManager.hpp"

#include "Common/Constants.hpp"

namespace Engine
{
    WindowManager::WindowManager(const std::string &title, int requestedWidth, int requestedHeight)
    {
        if (requestedWidth <= 0 || requestedHeight <= 0)
        {
            SDL_LogError(0, "Invalid window dimensions: %d x %d", requestedWidth, requestedHeight);
            return;
        }
        if (!SDL_Init(SDL_INIT_VIDEO))
        {
            SDL_LogError(0, "SDL not initialized");
            return;
        }

        m_sdlInitialized = true;

        int windowW = requestedWidth;
        int windowH = requestedHeight;
        SDL_Window *window = SDL_CreateWindow(title.c_str(), windowW, windowH, SDL_WINDOW_RESIZABLE);
        if (!window)
        {
            SDL_Quit();
            return;
        }

        m_window.reset(window);

        SDL_SetWindowMinimumSize(m_window.get(), Common::MINIMUM_SCREEN_WIDTH, Common::MINIMUM_SCREEN_HEIGHT);
        float aspect = (float)windowW / (float)windowH;
        SDL_SetWindowAspectRatio(m_window.get(), aspect, aspect);
    }

    WindowManager::~WindowManager()
    {
        if (m_sdlInitialized)
        {
            SDL_Quit();
        }
    }
} // namespace Engine