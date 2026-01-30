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
    };

    class WindowManager
    {
    public:
        WindowManager(const std::string &title, int width = 0, int height = 0);
        ~WindowManager();

        // Disable copying to prevent multiple windows or double-frees
        WindowManager(const WindowManager &) = delete;
        WindowManager &operator=(const WindowManager &) = delete;

        // Returns the underlying SDL_Window point. Caller must NOT free this pointer.
        SDL_Window *getSDLWindow() const { return m_window.get(); }

    private:
        std::unique_ptr<SDL_Window, SDLDeleter> m_window;
        bool m_sdlInitialized = false;
    };

} // namespace Engine
