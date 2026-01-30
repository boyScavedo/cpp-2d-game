#pragma once
#include <SDL3/SDL.h>
#include <memory>
#include <string>

#include "Engine/InputManager.hpp"

namespace Engine
{

    // Custom deleter: automatically calls SDL cleanup functions
    struct SDLDeleter
    {
        /**
         * @brief Destroys the given SDL_Window.
         *
         * Releases the SDL window resource referenced by `w`. If `w` is `nullptr`, this has no effect.
         *
         * @param w Pointer to the SDL_Window to destroy.
         */
        void operator()(SDL_Window *w) const { SDL_DestroyWindow(w); }
    };

    class WindowManager
    {
    public:
        WindowManager(const std::string &title, int width = 0, int height = 0);
        ~WindowManager();

        /**
         * @brief Deleted copy constructor to prohibit copying WindowManager instances.
         *
         * Prevents multiple owners of the managed SDL_Window and accidental double destruction.
         */
        WindowManager(const WindowManager &) = delete;
        /**
         * @brief Disable copy assignment for WindowManager.
         *
         * Prevents assigning one WindowManager to another to avoid multiple managers owning the same SDL_Window
         * and potential double-destruction of SDL resources.
         */
        WindowManager &operator=(const WindowManager &) = delete;

        /**
         * @brief Provides access to the managed SDL_Window.
         *
         * Returns the raw pointer to the SDL_Window owned by this WindowManager; ownership remains with the WindowManager.
         *
         * @return SDL_Window* Raw pointer to the managed SDL_Window, or `nullptr` if no window was created. Caller must not free or delete this pointer.
         */
        SDL_Window *getSDLWindow() const { return m_window.get(); }

        void update(const Common::InputState &input);
        void fpsCounter(Uint64 *currentTick, Uint64 *lastTime, Uint64 *fps);

    private:
        std::unique_ptr<SDL_Window, SDLDeleter> m_window;
        bool m_sdlInitialized = false;
    };

} // namespace Engine