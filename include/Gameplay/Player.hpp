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

        Player(float x, float y) : m_x(x), m_y(y) {}

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

        Common::RenderCommand getRenderCommand() const
        {
            return {m_x, m_y, WIDTH, HEIGHT, Common::TextureID::TEX_PLAYER}; // Texture Id at end
        }

    private:
        float m_x, m_y;
    };
} // namespace Gameplay