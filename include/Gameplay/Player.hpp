#pragma once
#include <SDL3/SDL.h>

namespace Gameplay
{
    class Player
    {
    public:
        /**
 * @brief Constructs a Player positioned at the given coordinates.
 *
 * Initializes the player's internal rectangle at (x, y) with a fixed size of 50x50 pixels.
 *
 * @param x Horizontal position in pixels.
 * @param y Vertical position in pixels.
 */
Player(float x, float y) : m_rect({x, y, 50.0f, 50.0f}) {}

        /**
         * @brief Moves the player according to WASD keyboard input.
         *
         * Adjusts the player's internal rectangle position (m_rect) when W/S/A/D are pressed.
         * Movement distance is scaled by the elapsed time parameter so motion is consistent
         * across frames (movement speed = 400.0f pixels per second).
         *
         * @param deltaTime Time elapsed since the last update, in seconds.
         */
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

        /**
         * @brief Renders the player as a filled green rectangle at its current position.
         *
         * @param renderer SDL_Renderer used to draw the player.
         */
        void draw(SDL_Renderer *renderer)
        {
            SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255); // Green square
            SDL_RenderFillRect(renderer, &m_rect);
        }

    private:
        SDL_FRect m_rect;
    };
}