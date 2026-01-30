#pragma once

#include "Gameplay/PlayerMovement.hpp"

#include "Common/Types.hpp"
#include "Common/Constants.hpp"

namespace Gameplay
{
    class Player
    {
    public:
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
        float width = Common::PLAYER_WIDTH;
        float height = Common::PLAYER_HEIGHT;
        PlayerMovement movement;
    };
} // namespace Gameplay