#include "Engine/WindowManager.hpp"
#include <iostream>

namespace Engine
{

    /**
     * @brief Initialize SDL, create a resizable window and renderer, and configure presentation and constraints.
     *
     * Constructs a WindowManager by initializing the SDL video subsystem, creating an SDL window and renderer,
     * configuring a 1280x720 logical presentation with letterboxing, setting a minimum window size (854x480),
     * and enforcing a square aspect ratio based on the created window's dimensions. If initialization or creation
     * fails, the instance is left in a non-running state.
     *
     * @param title Window title.
     * @param requestedWidth Requested window width; if zero, a runtime default of 854 is used.
     * @param requestedHeight Requested window height; if zero, a runtime default of 480 is used.
     */
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

    /**
     * @brief Cleans up WindowManager resources and shuts down SDL.
     *
     * Calls SDL_Quit to shut down any initialized SDL subsystems when the
     * WindowManager is destroyed.
     */
    WindowManager::~WindowManager()
    {
        SDL_Quit();
    }

    /**
     * @brief Process pending SDL events and update window and running state accordingly.
     *
     * Polls the SDL event queue, sets the internal running flag to false when a quit
     * event is received, and toggles the window fullscreen state when F11 is pressed.
     */
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

    /**
     * @brief Render a single frame to the managed SDL renderer.
     *
     * Clears the renderer to black, fills a 1280×720 rectangle at the origin with red,
     * and presents the resulting frame to the window.
     */
    void WindowManager::render()
    {
        // Set color to Red (R: 255, G: 0, B: 0, A: 255)
        SDL_SetRenderDrawColor(m_renderer.get(), 0, 0, 0, 255);
        SDL_RenderClear(m_renderer.get());

        SDL_SetRenderDrawColor(m_renderer.get(), 255, 0, 0, 255);

        SDL_FRect rect = {0, 0, 1280, 720};
        SDL_RenderFillRect(m_renderer.get(), &rect);

        SDL_RenderPresent(m_renderer.get());
    }

} // namespace Engine