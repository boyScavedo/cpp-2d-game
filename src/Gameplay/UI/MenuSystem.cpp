/**
 * @file MenuSystem.cpp
 * @brief Implementation of the MenuSystem class for handling game menus and user interface.
 *
 * This file provides the concrete implementation of menu rendering and interaction,
 * supporting both main menu and pause menu functionality with mouse-based controls.
 * The system handles button detection, menu state management, and command generation
 * for the rendering pipeline.
 */

#include "Gameplay/UI/MenuSystem.hpp"
#include <algorithm>
#include <iostream>

namespace Gameplay
{
    namespace UI
    {
        int MenuSystem::updateMainMenu(const Common::InputState &input, std::vector<Common::RenderCommand> &commands)
        {
            // Use actual window dimensions (MINIMUM values match window creation)
            float screenW = Common::SCREEN_WIDTH;
            float screenH = Common::SCREEN_HEIGHT;

            // Start Button (Green)
            float bw = Common::MENU_BUTTON_WIDTH, bh = Common::MENU_BUTTON_HEIGHT;
            float bx1 = (screenW - bw) / 2.0f;
            float by1 = (screenH / 2.0f) - 100.0f;

            commands.push_back({bx1, by1, bw, bh,
                                Common::TextureID::TEX_NONE, 0.0f,
                                0, 200, 0, 255});

            // Exit Button (Red)
            float bx2 = (screenW - bw) / 2.0f;
            float by2 = (screenH / 2.0f) + 20.0f;

            commands.push_back({bx2, by2, bw, bh,
                                Common::TextureID::TEX_NONE, 0.0f,
                                200, 0, 0, 255});

            static bool wasMousePressed = false;

            if (input.mouseLeftDown && !wasMousePressed)
            {
                wasMousePressed = true;
                if (isButtonClicked(input.mouseX, input.mouseY, bx1, by1, bw, bh))
                    return 1;
                if (isButtonClicked(input.mouseX, input.mouseY, bx2, by2, bw, bh))
                    return 2;
            }

            if (!input.mouseLeftDown)
            {
                wasMousePressed = false;
            }

            return 0;
        }

        int MenuSystem::updatePauseMenu(const Common::InputState &input, std::vector<Common::RenderCommand> &commands)
        {
            // Use actual window dimensions
            float screenW = Common::SCREEN_WIDTH;
            float screenH = Common::SCREEN_HEIGHT;

            // Dim background (Darker semi-transparent black)
            commands.push_back({0, 0, screenW, screenH,
                                Common::TextureID::TEX_NONE, 0.0f,
                                0, 0, 0, 220});

            // Centered Panel
            float pw = Common::PAUSE_PANEL_WIDTH, ph = Common::PAUSE_PANEL_HEIGHT;
            float px = (screenW - pw) / 2.0f;
            float py = (screenH - ph) / 2.0f;

            commands.push_back({
                px, py, pw, ph,
                Common::TextureID::TEX_NONE, 0.0f,
                50, 50, 50, 255 // Dark gray panel
            });

            // Button Dimensions
            float bw = Common::MENU_BUTTON_WIDTH, bh = Common::MENU_BUTTON_HEIGHT;
            float bx = (screenW - bw) / 2.0f;

            // Green Button (Resume)
            float by1 = py + 40.0f;
            commands.push_back({bx, by1, bw, bh,
                                Common::TextureID::TEX_NONE, 0.0f,
                                0, 180, 0, 255});

            // Blue Button (Main Menu)
            float by2 = py + 120.0f;
            commands.push_back({bx, by2, bw, bh,
                                Common::TextureID::TEX_NONE, 0.0f,
                                0, 100, 200, 255});

            // Red Button (Exit Game)
            float by3 = py + 200.0f;
            commands.push_back({bx, by3, bw, bh,
                                Common::TextureID::TEX_NONE, 0.0f,
                                180, 0, 0, 255});

            static bool wasMousePressed = false;

            if (input.mouseLeftDown && !wasMousePressed)
            {
                wasMousePressed = true;
                if (isButtonClicked(input.mouseX, input.mouseY, bx, by1, bw, bh))
                    return 1; // Resume
                if (isButtonClicked(input.mouseX, input.mouseY, bx, by2, bw, bh))
                    return 2; // Main Menu
                if (isButtonClicked(input.mouseX, input.mouseY, bx, by3, bw, bh))
                    return 3; // Exit
            }

            if (!input.mouseLeftDown)
            {
                wasMousePressed = false;
            }

            return 0;
        }

        bool MenuSystem::isButtonClicked(int mx, int my, float bx, float by, float bw, float bh)
        {
            return (mx >= bx && mx <= bx + bw && my >= by && my <= by + bh);
        }
    }
}
