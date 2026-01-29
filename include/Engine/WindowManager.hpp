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

        // Disable copying to prevent multiple windows or double-frees
        WindowManager(const WindowManager &) = delete;
        WindowManager &operator=(const WindowManager &) = delete;

        bool isRunning() const { return m_running; }
        SDL_Renderer *getRenderer() const { return m_renderer.get(); }
        void pollEvents();
        void render();

    private:
        std::unique_ptr<SDL_Window, SDLDeleter> m_window;
        std::unique_ptr<SDL_Renderer, SDLDeleter> m_renderer;
        bool m_running = true;
    };

} // namespace Engine