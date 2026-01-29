#pragma once
#include <SDL3/SDL.h>

namespace Gameplay
{
    class Player
    {
    public:
        static constexpr float WIDTH = 50.0f;
        static constexpr float HEIGHT = 50.0f;

        Player(float x, float y) : m_rect({x, y, WIDTH, HEIGHT}) {}

        void update(float deltaTime)
        {
            const float speed = 400.0f; // Pixels per second
            const bool *state = SDL_GetKeyboardState(NULL);

            if (state[SDL_SCANCODE_W])
                m_rect.y -= speed * deltaTime;
            if (state[SDL_SCANCODE_S])
                m_rect.y += speed * deltaTime;
            if (state[SDL_SCANCODE_A])
                m_rect.x -= speed * deltaTime;
            if (state[SDL_SCANCODE_D])
                m_rect.x += speed * deltaTime;

            if (m_rect.x < 0)
            {
                m_rect.x = 0;
            }
            if (m_rect.x > 1280 - WIDTH)
            {
                m_rect.x = 1280 - WIDTH;
            }
            if (m_rect.y < 0)
            {
                m_rect.y = 0;
            }
            if (m_rect.y > 720 - HEIGHT)
            {
                m_rect.y = 720 - HEIGHT;
            }
        }

        void draw(SDL_Renderer *renderer)
        {
            if (!renderer)
            {
                return;
            }
            SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255); // Green square
            SDL_RenderFillRect(renderer, &m_rect);
        }

    private:
        SDL_FRect m_rect;
    };
}