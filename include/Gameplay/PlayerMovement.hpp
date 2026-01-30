#pragma once

#include <utility>

#include "Common/Types.hpp"
#include "Common/Constants.hpp"

namespace Gameplay
{
    /**
     * @class PlayerMovement
     * @brief Handles the physics and movement logic for the player character.
     *
     * Manages position, velocity, acceleration, friction, gravity, and jumping mechanics
     * to simulate realistic 2D platformer movement.
     */
    class PlayerMovement
    {
    private:
        const float acceleration = Common::PLAYER_ACCELERATION;
        const float friction = Common::FRICTION; // higher = less slippery
        const float gravity = Common::GRAVITY;
        const float terminalVelocity = Common::TERMINAL_VELOCITY;
        const float jumpForce = Common::JUMP_FORCE;
        const float maxSpeed = Common::PLAYER_MAX_SPEED; // Pixels per second

        float m_x = 0, m_y = 0;
        float m_velocityX = 0, m_velocityY = 0;
        bool m_canJump = false;

    public:
        /**
         * @brief Updates the player's movement based on input and time elapsed.
         * @param deltaTime Time elapsed since the last update, in seconds.
         * @param input The current input state to influence movement.
         */
        void update(float deltaTime, const Common::InputState &input);

        /**
         * @brief Builds a render command for the player using its current position, size, and player texture.
         * @return Common::RenderCommand A command containing position (x, y), size (width, height), and the `Common::TextureID::TEX_PLAYER` texture identifier.
         */
        Common::RenderCommand getRenderCommand() const;

        /**
         * @brief Gets the player's current position.
         * @return std::pair<float, float> A pair containing the x and y coordinates of the player.
         */
        std::pair<float, float> getPosition() const;
    };
};
