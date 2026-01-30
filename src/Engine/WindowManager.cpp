#include <string>

#include "Engine/WindowManager.hpp"
#include "Engine/InputManager.hpp"

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
            m_sdlInitialized = false;
            SDL_Quit();
            return;
        }

        m_window.reset(window);

        SDL_SetWindowMinimumSize(m_window.get(), Common::MINIMUM_SCREEN_WIDTH, Common::MINIMUM_SCREEN_HEIGHT);
        float aspect = (float)windowW / (float)windowH;
        SDL_SetWindowAspectRatio(m_window.get(), aspect, aspect);
    }

    /**
     * @brief Toggles the window to be either fullscreen or windowed
     *
     * @param input The current input state of the player.
     */
    void WindowManager::update(const Common::InputState &input)
    {
        SDL_Window *window = getSDLWindow();
        if (input.toggleFullScreen)
        {
            Uint32 flags = SDL_GetWindowFlags(window);
            if (flags & SDL_WINDOW_FULLSCREEN)
            {
                SDL_SetWindowFullscreen(window, false);
            }
            else
            {
                SDL_SetWindowFullscreen(window, true);
            }
        }
    }

    void WindowManager::fpsCounter(Uint64 *currentTime, Uint64 *lastFpsUpdate, Uint64 *fps)
    {
        (*fps)++;

        if (*currentTime > *lastFpsUpdate + 1000)
        {
            SDL_Window *window = getSDLWindow();
            *lastFpsUpdate = *currentTime;
            std::string title = Common::WINDOW_TITLE + std::to_string(*fps);
            SDL_SetWindowTitle(window, title.c_str());
            *fps = 0;
        }
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