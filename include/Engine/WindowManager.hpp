#pragma once
#include <SDL3/SDL.h>
#include <memory>
#include <string>

#include "Engine/InputManager.hpp"

namespace Engine
{
    /**
     * @struct SDLDeleter
     * @brief Custom deleter for std::unique_ptr to automatically manage SDL_Window lifecycle.
     *
     * This helper struct ensures proper cleanup of SDL resources by calling SDL_DestroyWindow
     * when the unique_ptr goes out of scope, preventing memory leaks and resource conflicts.
     * It integrates RAII principles into the WindowManager, making SDL resource management safer.
     */
    struct SDLDeleter
    {
        /**
         * @brief Destroys the given SDL_Window.
         *
         * Releases the SDL window resource referenced by `w`. If `w` is `nullptr`, this has no effect.
         * This function is called automatically by the unique_ptr when the WindowManager is destroyed.
         *
         * @param w Pointer to the SDL_Window to destroy.
         */
        void operator()(SDL_Window *w) const { SDL_DestroyWindow(w); }
    };

    /**
     * @class WindowManager
     * @brief Manages the application window and SDL initialization, providing a high-level interface for window operations.
     *
     * The WindowManager is a core Engine component responsible for creating and managing the SDL window,
     * handling fullscreen toggles, and providing access to the window for rendering. It encapsulates SDL
     * initialization and cleanup, ensuring the game can run in both windowed and fullscreen modes.
     * This class interacts closely with the Renderer (which uses the SDL_Window) and InputManager
     * (for handling window-related events), forming the foundation of the game's display system.
     * By using RAII with unique_ptr, it guarantees resource safety and prevents common SDL pitfalls.
     */
    class WindowManager
    {
    public:
        /**
         * @brief Constructs a WindowManager with the specified title and optional dimensions.
         *
         * Initializes SDL if not already done, creates the window with the given parameters,
         * and sets up the window for rendering. If width/height are 0, defaults to fullscreen or system settings.
         * This constructor establishes the visual output channel for the entire game.
         *
         * @param title The title of the window, displayed in the OS window bar.
         * @param width The width of the window in pixels (0 for default/fullscreen).
         * @param height The height of the window in pixels (0 for default/fullscreen).
         */
        WindowManager(const std::string &title, int width = 0, int height = 0);

        /**
         * @brief Destroys the WindowManager and cleans up SDL resources.
         *
         * Automatically destroys the SDL_Window via the custom deleter and shuts down SDL if initialized here.
         * This ensures no resource leaks when the game exits or switches contexts (e.g., back to CLI).
         */
        ~WindowManager();

        /**
         * @brief Deleted copy constructor to prohibit copying WindowManager instances.
         *
         * Prevents multiple owners of the managed SDL_Window and accidental double destruction,
         * enforcing single ownership for SDL resource safety.
         */
        WindowManager(const WindowManager &) = delete;

        /**
         * @brief Deleted copy assignment operator for WindowManager.
         *
         * Prevents assigning one WindowManager to another to avoid multiple managers owning the same SDL_Window
         * and potential double-destruction of SDL resources, maintaining RAII integrity.
         */
        WindowManager &operator=(const WindowManager &) = delete;

        /**
         * @brief Provides access to the managed SDL_Window.
         *
         * Returns the raw pointer to the SDL_Window owned by this WindowManager; ownership remains with the WindowManager.
         * This is used by the Renderer to create an SDL_Renderer tied to the window.
         *
         * @return SDL_Window* Raw pointer to the managed SDL_Window, or `nullptr` if no window was created. Caller must not free or delete this pointer.
         */
        SDL_Window *getSDLWindow() const { return m_window.get(); }

        /**
         * @brief Updates the window state based on input, handling fullscreen toggles.
         *
         * Processes input events related to window management, such as toggling fullscreen mode.
         * This method integrates with the InputManager to respond to user commands, enhancing the player's control over display settings.
         *
         * @param input The current input state, checked for fullscreen toggle requests.
         */
        void update(const Common::InputState &input);

        /**
         * @brief Updates the FPS counter and computes frames per second.
         *
         * Tracks time between frames to calculate and update the FPS value, which is displayed in the window title.
         * This provides real-time performance feedback, helping monitor game smoothness and identify bottlenecks.
         *
         * @param currentTick Current SDL tick count, used for timing calculations.
         * @param lastTime Reference to the last recorded time, updated by this method.
         * @param fps Reference to the computed frames per second, updated by this method.
         */
        void fpsCounter(const Uint64 &currentTick, Uint64 &lastTime, Uint64 &fps);

    private:
        std::unique_ptr<SDL_Window, SDLDeleter> m_window; /**< Managed SDL_Window with automatic cleanup. */
        bool m_sdlInitialized = false;                    /**< Flag indicating if SDL was initialized by this instance. */
    };

} // namespace Engine
