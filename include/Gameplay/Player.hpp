#pragma once
#include "Common/Types.hpp"
#include "Common/Constants.hpp"

namespace Gameplay
{
    class Player
    {
    public:
        static constexpr float WIDTH = 50.0f;
        static constexpr float HEIGHT = 50.0f;

        /**
 * @brief Constructs a Player at the given position.
 *
 * @param x Initial x-coordinate (pixels from left).
 * @param y Initial y-coordinate (pixels from top).
 */
Player(float x, float y) : m_x(x), m_y(y) {}

        /**
         * @brief Update the player's position based on input and elapsed time, clamping it to the screen area.
         *
         * Moves the player by a fixed speed (400 pixels per second) scaled by `deltaTime` when the corresponding
         * direction flags in `input` are set, then clamps the resulting position so the player remains fully
         * within [0, SCREEN_WIDTH - WIDTH] horizontally and [0, SCREEN_HEIGHT - HEIGHT] vertically.
         *
         * @param deltaTime Time elapsed since the last update, in seconds.
         * @param input Structure containing directional input flags (`up`, `down`, `left`, `right`).
         */
        void update(float deltaTime, const Common::InputState &input)
        {
            const float speed = 400.0f; // Pixels per second

            if (input.up)
                m_y -= speed * deltaTime;
            if (input.down)
                m_y += speed * deltaTime;
            if (input.left)
                m_x -= speed * deltaTime;
            if (input.right)
                m_x += speed * deltaTime;

            if (m_x < 0)
            {
                m_x = 0;
            }
            if (m_x > Common::SCREEN_WIDTH - WIDTH)
            {
                m_x = Common::SCREEN_WIDTH - WIDTH;
            }
            if (m_y < 0)
            {
                m_y = 0;
            }
            if (m_y > Common::SCREEN_HEIGHT - HEIGHT)
            {
                m_y = Common::SCREEN_HEIGHT - HEIGHT;
            }
        }

        /**
         * @brief Builds a render command for the player using its current position, size, and player texture.
         *
         * @return Common::RenderCommand A command containing position (x, y), size (width, height), and the `Common::TextureID::TEX_PLAYER` texture identifier.
         */
        Common::RenderCommand getRenderCommand() const
        {
            return {m_x, m_y, WIDTH, HEIGHT, Common::TextureID::TEX_PLAYER}; // Texture Id at end
        }

    private:
        float m_x, m_y;
    };
} // namespace Gameplay