#pragma once
#include <SDL3/SDL.h>

namespace Gameplay
{
    class Player
    {
    public:
        Player(float x, float y) : m_rect({x, y, 50.0f, 50.0f}) {}

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
        }

        void draw(SDL_Renderer *renderer)
        {
            SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255); // Green square
            SDL_RenderFillRect(renderer, &m_rect);
        }

    private:
        SDL_FRect m_rect;
    };
}