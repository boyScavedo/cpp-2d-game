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
             * @brief Renders the progress bar as render commands.
             *
             * Generates render commands to draw the progress bar background and fill.
             * The bar consists of a background rectangle and a filled portion representing
             * current progress. Commands are appended to the provided vector for batch processing.
             *
             * @param commands Vector of render commands to append progress bar elements to.
             */
            void render(std::vector<Common::RenderCommand> &commands) const;

        private:
            float m_x, m_y;
            float m_width, m_height;
            float m_fillWidth = 0.0f;
        };
    }
}
