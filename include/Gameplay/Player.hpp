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

        Player(float x, float y) : m_x(x), m_y(y), m_velocityY(0.0f) {}

        void update(float deltaTime, const Common::InputState &input);

        Common::RenderCommand getRenderCommand() const;

    private:
        float m_x, m_y;
        float m_velocityX = 0, m_velocityY = 0;
        bool canJump = false;
    };
} // namespace Gameplay