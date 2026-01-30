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
        static constexpr float GRAVITY = 980.0f;       // Pixels per second squared
        static constexpr float JUMP_VELOCITY = 500.0f; // Pixels per second

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
        void update(float deltaTime, const Common::InputState &input);

        /**
         * @brief Builds a render command for the player using its current position, size, and player texture.
         *
         * @return Common::RenderCommand A command containing position (x, y), size (width, height), and the `Common::TextureID::TEX_PLAYER` texture identifier.
         */
        Common::RenderCommand getRenderCommand() const;

    private:
        float m_x, m_y;
        float m_velocityX = 0, m_velocityY = 0;
        bool canJump = false;
    };
} // namespace Gameplay