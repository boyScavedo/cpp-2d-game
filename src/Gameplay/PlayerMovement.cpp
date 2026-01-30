#include "Gameplay/PlayerMovement.hpp"

void Gameplay::PlayerMovement::update(float deltaTime, const Common::InputState &input)
{
    if (input.left)
    {
        m_velocityX += -(acceleration * deltaTime);
        m_velocityY += gravity * deltaTime;
        if (m_velocityX >= 0)
            m_velocityX += -(friction * deltaTime);
        if (m_velocityX < -maxSpeed)
            m_velocityX = -maxSpeed;
    }

    else if (input.right)
    {
        m_velocityX += (acceleration * deltaTime);
        m_velocityY += gravity * deltaTime;
        if (m_velocityX <= 0)
            m_velocityX += friction * deltaTime;

        if (m_velocityX > maxSpeed)
            m_velocityX = maxSpeed;
    }
    else
    {
        m_velocityY += gravity * deltaTime;

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
    if (m_velocityY > terminalVelocity)
        m_velocityY = terminalVelocity;

    if (input.jump && m_canJump)
    {
        m_velocityY = jumpForce;
        m_canJump = false;
    }

    m_x += m_velocityX * deltaTime;
    m_y += m_velocityY * deltaTime;

    if (m_x < 0)
    {
        m_x = 0;
    }
    if (m_x > Common::SCREEN_WIDTH - Common::PLAYER_WIDTH)
    {
        m_x = Common::SCREEN_WIDTH - Common::PLAYER_WIDTH;
    }
    if (m_y < 0)
    {
        m_y = 0;
    }
    if (m_y > Common::SCREEN_HEIGHT - Common::PLAYER_HEIGHT)
    {
        m_y = Common::SCREEN_HEIGHT - Common::PLAYER_HEIGHT;
        m_canJump = true;
    }
}

Common::RenderCommand Gameplay::PlayerMovement::getRenderCommand() const
{
    return {m_x, m_y, Common::PLAYER_WIDTH, Common::PLAYER_HEIGHT, Common::TextureID::TEX_PLAYER}; // Texture Id at end
}