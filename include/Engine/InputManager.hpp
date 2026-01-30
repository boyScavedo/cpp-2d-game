#pragma once
#include <SDL3/SDL.h>
#include "Common/Types.hpp"

namespace Engine
{
    class InputManager
    {
    private:
        Common::InputState m_state{};

    public:
        /**
         * @brief Polls SDL events and updates the stored input state.
         *
         * Updates movement and action flags (up, down, left, right, attack) from the current keyboard state
         * and sets the `quit` flag if an SDL quit event is received.
         *
         * @return Common::InputState The updated input state containing the current values of
         * `up`, `down`, `left`, `right`, `attack`, and `quit`.
         */
        Common::InputState update()
        {
            m_state.toggleFullScreen = false;
            SDL_Event event;

            while (SDL_PollEvent(&event))
            {
                if (event.type == SDL_EVENT_QUIT)
                    m_state.quit = true;
                if (event.type == SDL_EVENT_KEY_DOWN)
                {
                    if (event.key.scancode == SDL_SCANCODE_F11)
                    {
                        m_state.toggleFullScreen = true;
                    }
                }
            }
            const bool *keys = SDL_GetKeyboardState(nullptr);
            m_state.up = keys[SDL_SCANCODE_W];
            m_state.down = keys[SDL_SCANCODE_S];
            m_state.left = keys[SDL_SCANCODE_A];
            m_state.right = keys[SDL_SCANCODE_D];
            m_state.jump = keys[SDL_SCANCODE_SPACE];

            SDL_MouseButtonFlags mouseButtons = SDL_GetMouseState(nullptr, nullptr);
            m_state.attack = (mouseButtons & SDL_BUTTON_MASK(SDL_BUTTON_LEFT)) != 0;

            return m_state;
        }
    };
}