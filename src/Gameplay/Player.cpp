#include "Gameplay/Player.hpp"

#include "Common/Types.hpp"
#include "Common/Constants.hpp"

void Gameplay::Player::update(float deltaTime, const Common::InputState &input)
{
    const float maxSpeed = 500.0f; // Pixels per second
    const float accelaration = 5000.0f;
    const float friction = 10000.0f; // higher = less slippery
    const float gravity = Common::GRAVITY;
    const float jumpForce = -500.0f;

    if (input.left)
    {
        m_velocityX += -(accelaration * deltaTime);
        m_velocityY += gravity * deltaTime;
        if (m_velocityX >= 0)
            m_velocityX += -(friction * deltaTime);
        if (m_velocityX < -maxSpeed)
            m_velocityX = -maxSpeed;
    }

    else if (input.right)
    {
        m_velocityX += (accelaration * deltaTime);
        m_velocityY += gravity * deltaTime;
        if (m_velocityX <= 0)
            m_velocityX += friction * deltaTime;

        if (m_velocityX > maxSpeed)
            m_velocityX = maxSpeed;
    }
    else
    {
        m_velocityY += gravity * deltaTime;

        if (m_velocityY > gravity)
            m_velocityY = gravity;

        if (m_velocityX > 0)
        {
            m_velocityX -= friction * deltaTime;
            if (m_velocityX < 0)
                m_velocityX = 0;
        }
        else if (m_velocityX < 0)
        {
            m_velocityX += friction * deltaTime;
            if (m_velocityX > 0)
                m_velocityX = 0;
        }
    }

    if (input.jump && canJump)
    {
        m_velocityY = jumpForce;
        canJump = false;
    }

    m_x += m_velocityX * deltaTime;
    m_y += m_velocityY * deltaTime;

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
        canJump = true;
    }
}

Common::RenderCommand Gameplay::Player::getRenderCommand() const
{
    return {m_x, m_y, WIDTH, HEIGHT, Common::TextureID::TEX_PLAYER}; // Texture Id at end
}