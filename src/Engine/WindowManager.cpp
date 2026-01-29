#include "Engine/WindowManager.hpp"

namespace Engine
{

    WindowManager::WindowManager(const std::string &title, int requestedWidth, int requestedHeight)
    {
        if (!SDL_Init(SDL_INIT_VIDEO))
        {
            m_running = false;
            return;
        }

        int windowW = requestedWidth;
        int windowH = requestedHeight;

        // 1. Runtime Detection
        if (windowW == 0 || windowH == 0)
        {
            windowW = 854;
            windowH = 480;
        }

        // 2. Create Window
        SDL_Window *window;
        SDL_Renderer *renderer;
        if (!SDL_CreateWindowAndRenderer(title.c_str(), windowW, windowH, SDL_WINDOW_RESIZABLE, &window, &renderer))
        {
            m_running = false;
            return;
        }

        m_window.reset(window);
        m_renderer.reset(renderer);

        // 3. The "Magic" Scaling (SDL3 feature)
        // This tells SDL: "I want to draw as if the screen is 1280x720,
        // you handle the stretching/letterboxing to fit the actual window."
        SDL_SetRenderLogicalPresentation(
            m_renderer.get(),
            1280, 720,
            SDL_LOGICAL_PRESENTATION_LETTERBOX);

        SDL_SetWindowMinimumSize(m_window.get(), 854, 480);
        float aspect = (float)windowW / (float)windowH;
        SDL_SetWindowAspectRatio(m_window.get(), aspect, aspect);

        m_running = true;
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

            if (event.type == SDL_EVENT_KEY_DOWN)
            {
                if (event.key.key == SDLK_F11)
                {
                    Uint32 flags = SDL_GetWindowFlags(m_window.get());
                    bool isFullscreen = (flags & SDL_WINDOW_FULLSCREEN);
                    SDL_SetWindowFullscreen(m_window.get(), !isFullscreen);
                }
            }
        }
    }

} // namespace Engine