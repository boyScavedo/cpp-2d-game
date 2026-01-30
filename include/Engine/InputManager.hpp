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
            SDL_Event event;

            while (SDL_PollEvent(&event))
            {
                if (event.type == SDL_EVENT_QUIT)
                    m_state.quit = true;
            }
            const bool *keys = SDL_GetKeyboardState(nullptr);
            m_state.up = keys[SDL_SCANCODE_W];
            m_state.down = keys[SDL_SCANCODE_S];
            m_state.left = keys[SDL_SCANCODE_A];
            m_state.right = keys[SDL_SCANCODE_D];
            m_state.attack = keys[SDL_SCANCODE_SPACE];

            return m_state;
        }
    };
}