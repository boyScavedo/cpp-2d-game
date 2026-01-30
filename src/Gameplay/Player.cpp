#include "Gameplay/Player.hpp"

#include "Common/Types.hpp"
#include "Common/Constants.hpp"

void Gameplay::Player::update(float deltaTime, const Common::InputState &input)
{
    movement.update(deltaTime, input);
}

Common::RenderCommand Gameplay::Player::getRenderCommand() const
{
    return movement.getRenderCommand(); // Texture Id at end
}
