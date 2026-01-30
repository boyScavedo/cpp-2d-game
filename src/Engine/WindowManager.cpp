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
     * @brief Toggle the managed window between fullscreen and windowed modes when requested.
     *
     * If a valid SDL_Window exists and the provided input requests a fullscreen toggle,
     * this method switches the window's fullscreen state.
     *
     * @param input Input state whose `toggleFullScreen` flag triggers the toggle when true.
     */
    void WindowManager::update(const Common::InputState &input)
    {
        SDL_Window *window = getSDLWindow();

        if (!window)
            return;

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

    /**
     * @brief Updates the frame rate counter and refreshes the window title once per second.
     *
     * Increments the supplied frame counter and, when more than 1000 milliseconds have
     * elapsed since lastFpsUpdate, sets the SDL window title to Common::WINDOW_TITLE_PREFIX
     * followed by the current FPS value, resets the frame counter to zero, and updates
     * lastFpsUpdate to the current time.
     *
     * @param currentTime Current time in milliseconds.
     * @param lastFpsUpdate Reference to the timestamp of the last title update; updated to currentTime when a title refresh occurs.
     * @param fps Reference to the accumulated frame count; incremented on each call and reset to 0 after a title refresh.
     */
    void WindowManager::fpsCounter(const Uint64 &currentTime, Uint64 &lastFpsUpdate, Uint64 &fps)
    {
        fps++;

        if (currentTime > lastFpsUpdate + 1000)
        {
            SDL_Window *window = getSDLWindow();

            if (!window)
                return;

            lastFpsUpdate = currentTime;
            std::string title = Common::WINDOW_TITLE_PREFIX + std::to_string(fps);
            SDL_SetWindowTitle(window, title.c_str());
            fps = 0;
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