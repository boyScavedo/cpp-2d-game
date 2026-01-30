#include "Engine/WindowManager.hpp"

#include "Common/Constants.hpp"

namespace Engine
{
    /**
     * @brief Initialize the WindowManager and create an SDL window with the requested size.
     *
     * Validates the requested dimensions, initializes SDL's video subsystem, creates a resizable
     * SDL_Window, and configures the window's minimum size and aspect ratio. If validation,
     * SDL initialization, or window creation fails this constructor returns early; on window
     * creation failure SDL is shut down via SDL_Quit().
     *
     * @param title Window title shown in the window decorations.
     * @param requestedWidth Desired window width in pixels; must be greater than 0.
     * @param requestedHeight Desired window height in pixels; must be greater than 0.
     */
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

    /**
     * @brief Cleans up WindowManager resources.
     *
     * If the SDL subsystem was initialized by this manager, shuts down SDL by calling SDL_Quit().
     */
    WindowManager::~WindowManager()
    {
        if (m_sdlInitialized)
        {
            SDL_Quit();
        }
    }
} // namespace Engine