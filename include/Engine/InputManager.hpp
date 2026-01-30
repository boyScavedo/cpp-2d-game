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