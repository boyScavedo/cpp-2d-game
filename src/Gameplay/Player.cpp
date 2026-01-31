#include "Gameplay/Player.hpp"

#include <utility>

#include "Common/Types.hpp"
#include "Common/Constants.hpp"

/**
 * @brief Update the player's movement state for the current frame.
 *
 * Apply per-frame updates to the player's movement component using the elapsed
 * time and the current input state.
 *
 * @param deltaTime Time elapsed since the last update, in seconds.
 * @param input Current input state to influence movement.
 */
void Gameplay::Player::update(float deltaTime, const Common::InputState &input)
{
    movement.update(deltaTime, input);
}

/**
 * @brief Retrieves the render command representing the player's current visual state.
 *
 * The command is obtained from the player's movement component; the texture ID is stored as the last element of the returned command.
 *
 * @return Common::RenderCommand Render command for the player.
 */
Common::RenderCommand Gameplay::Player::getRenderCommand() const
{
    return movement.getRenderCommand(); // Texture Id at end
}

/**
 * @brief Gets the player's current position.
 *
 * @return std::pair<float, float> A pair containing the x and y coordinates of the player.
 */
std::pair<float, float> Gameplay::Player::getPosition() const
{
    return movement.getPosition();
}
