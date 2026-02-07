#pragma once

#include <vector>
#include "Common/Types.hpp"

namespace Gameplay
{
    namespace UI
    {
        /**
         * @class ProgressIndicator
         * @brief Manages and renders the UI element showing the player's progress through the level.
         */
        class ProgressIndicator
        {
        public:
            /**
             * @brief Constructs a ProgressIndicator with initial position and size.
             * @param x Horizontal position on screen.
             * @param y Vertical position on screen.
             * @param width Total width of the progress bar.
             * @param height Height of the progress bar.
             */
            ProgressIndicator(float x, float y, float width, float height);

            /**
             * @brief Updates the progress based on the player's position.
             * @param playerX The current X position of the player.
             * @param worldWidth The total width of the current level.
             */
            void update(float playerX, float worldWidth);

            /**
             * @brief Adds the necessary render commands to the frame's command list.
             * @param commands The vector of render commands to append to.
             */
            void render(std::vector<Common::RenderCommand>& commands) const;

        private:
            float m_x, m_y;
            float m_width, m_height;
            float m_fillWidth = 0.0f;
        };
    }
}
