/**
 * @file ProgressIndicator.cpp
 * @brief Implementation of the ProgressIndicator class for displaying level progress.
 *
 * This file provides the concrete implementation of the progress bar UI element,
 * calculating and rendering the player's advancement through the current level
 * as a visual indicator. The progress is based on player position relative to
 * level width and displayed as a filled horizontal bar.
 */

#include "Gameplay/UI/ProgressIndicator.hpp"
#include "Common/Constants.hpp"
#include <algorithm>

namespace Gameplay
{
    namespace UI
    {
        ProgressIndicator::ProgressIndicator(float x, float y, float width, float height)
            : m_x(x), m_y(y), m_width(width), m_height(height)
        {
        }

        void ProgressIndicator::update(float playerX, float worldWidth)
        {
            if (worldWidth <= 0.0f)
            {
                m_fillWidth = 0.0f;
                return;
            }

            float ratio = std::clamp(playerX / worldWidth, 0.0f, 1.0f);
            m_fillWidth = ratio * m_width;
        }

        void ProgressIndicator::render(std::vector<Common::RenderCommand> &commands) const
        {
            // 1. Draw Background Bar
            commands.push_back({
                .x = m_x,
                .y = m_y,
                .width = m_width,
                .height = m_height,
                .textureID = Common::TextureID::TEX_NONE, // Typically a dark gray or black
                .scrollFactor = 0.0f                      // Static UI element
            });

            // 2. Draw Progress Fill
            if (m_fillWidth > 0.0f)
            {
                commands.push_back({.x = m_x,
                                    .y = m_y,
                                    .width = m_fillWidth,
                                    .height = m_height,
                                    .textureID = Common::TextureID::TEX_PLAYER, // Using player theme for progress
                                    .scrollFactor = 0.0f});
            }
        }
    }
}
