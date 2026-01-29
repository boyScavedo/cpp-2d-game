#pragma once
#include <SDL3/SDL.h>
#include <memory>
#include <string>

namespace Engine
{

    // Custom deleter: automatically calls SDL cleanup functions
    struct SDLDeleter
    {
        void operator()(SDL_Window *w) const { SDL_DestroyWindow(w); }
        void operator()(SDL_Renderer *r) const { SDL_DestroyRenderer(r); }
    };

    class WindowManager
    {
    public:
        WindowManager(const std::string &title, int width = 0, int height = 0);
        ~WindowManager();

        /**
 * @brief Disables copy construction to prevent creating multiple managers that would share or double-free SDL resources.
 */
        WindowManager(const WindowManager &) = delete;
        /**
 * @brief Disable copy assignment for WindowManager.
 *
 * Prevents assigning one WindowManager to another to avoid duplicating ownership of
 * the underlying SDL resources and the risk of double destruction.
 */
WindowManager &operator=(const WindowManager &) = delete;

        /**
 * @brief Indicates whether the window manager is currently running.
 *
 * @return `true` if the window manager is running, `false` otherwise.
 */
bool isRunning() const { return m_running; }
        /**
 * @brief Accesses the SDL renderer managed by the WindowManager.
 *
 * Returns a raw pointer to the internally managed SDL_Renderer instance.
 *
 * @return SDL_Renderer* Pointer to the managed renderer, or `nullptr` if no renderer is initialized.
 */
SDL_Renderer *getRenderer() const { return m_renderer.get(); }
        void pollEvents();
        void render();

    private:
        std::unique_ptr<SDL_Window, SDLDeleter> m_window;
        std::unique_ptr<SDL_Renderer, SDLDeleter> m_renderer;
        bool m_running = true;
    };

} // namespace Engine