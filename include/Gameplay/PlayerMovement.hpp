#pragma once

#include "Common/Types.hpp"
#include "Common/Constants.hpp"

namespace Gameplay
{
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
        void update(float deltaTime, const Common::InputState &input);

        Common::RenderCommand getRenderCommand() const;
    };
};
