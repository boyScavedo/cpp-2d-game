#include <utility>

#include "Gameplay/PlayerMovement.hpp"

/**
 * @brief Updates the player's physics state and position based on input and elapsed time.
 *
 * Updates horizontal and vertical velocity using acceleration, gravity, and friction; clamps
 * velocities to configured limits; applies a jump when allowed; integrates position; and
 * enforces screen bounds. Landing on the bottom edge re-enables jumping.
 *
 * @param deltaTime Time elapsed since the last update in seconds.
 * @param input Input state containing movement flags (`left`, `right`, `jump`) that drive motion.
 */
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
        m_velocityX = 0;
    }
    if (m_x > Common::WORLD_WIDTH - Common::PLAYER_WIDTH)
    {
        m_x = Common::WORLD_WIDTH - Common::PLAYER_WIDTH;
        m_velocityX = 0;
    }
    if (m_y < 0)
    {
        m_y = 0;
        m_velocityY = 0;
    }
    if (m_y > Common::SCREEN_HEIGHT - Common::PLAYER_HEIGHT)
    {
        m_y = Common::SCREEN_HEIGHT - Common::PLAYER_HEIGHT;
        m_velocityY = 0;
        m_canJump = true;
    }
}

/**
 * @brief Produces a render command for the player using its current position and size.
 *
 * @return Common::RenderCommand A render command initialized with the player's x and y position,
 * width (Common::PLAYER_WIDTH), height (Common::PLAYER_HEIGHT), and texture ID (Common::TextureID::TEX_PLAYER).
 */
Common::RenderCommand Gameplay::PlayerMovement::getRenderCommand() const
{
    return {m_x, m_y, Common::PLAYER_WIDTH, Common::PLAYER_HEIGHT, Common::TextureID::TEX_PLAYER}; // Texture Id at end
}

/**
 * @brief Gets the player's current position.
 *
 * @return std::pair<float, float> A pair containing the x and y coordinates of the player.
 */
std::pair<float, float> Gameplay::PlayerMovement::getPosition() const
{
    return {m_x, m_y};
}
