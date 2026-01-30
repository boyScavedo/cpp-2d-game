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
         * @brief Polls system input and updates the stored input state.
         *
         * Updates directional and action flags and records high-level input events:
         * - Sets `quit` when a quit event is received.
         * - Sets `toggleFullScreen` when the F11 key is pressed.
         * - Updates `up`, `down`, `left`, `right` from the W/S/A/D keys.
         * - Updates `jump` from the Space key.
         * - Updates `attack` from the left mouse button.
         *
         * @return Common::InputState The updated input state reflecting the current values of
         * `up`, `down`, `left`, `right`, `jump`, `attack`, `toggleFullScreen`, and `quit`.
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