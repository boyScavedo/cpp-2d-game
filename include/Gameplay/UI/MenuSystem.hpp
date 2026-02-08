#pragma once

#include <vector>
#include <string>
#include "Common/Types.hpp"

namespace Gameplay
{
    namespace UI
    {
        /**
         * @class MenuSystem
         * @brief Manages user interface menus for game state transitions and navigation.
         *
         * MenuSystem provides the visual and interactive components for the game's menu system,
         * handling both the initial main menu and the in-game pause menu. It integrates with the
         * game's state machine, processing user input to trigger state changes like starting gameplay,
         * resuming from pause, or exiting the application. The system uses simple button-based
         * interaction with mouse input, rendering menu elements as colored rectangles with text.
         * This keeps the UI lightweight while providing essential navigation functionality.
         */
        class MenuSystem
        {
        public:
            MenuSystem() = default; /**< Default constructor for menu system initialization. */

            /**
             * @brief Processes input and renders the Main Menu for game startup options.
             *
             * Displays the primary menu shown when the game launches, offering options to start
             * playing or exit. Handles mouse input for button selection and returns action codes
             * to drive the main game loop state transitions. Renders menu buttons and background
             * using render commands that are processed by the Renderer.
             *
             * @param input Current input state for mouse interaction detection.
             * @param commands Vector of render commands to append menu elements to.
             * @return 1 for Start Game action, 2 for Exit action, 0 for no action taken.
             */
            int updateMainMenu(const Common::InputState &input, std::vector<Common::RenderCommand> &commands);

            /**
             * @brief Processes input and renders the Pause Menu for in-game options.
             *
             * Shows the pause menu overlay during gameplay, allowing players to resume,
             * return to main menu, or quit. Maintains game state while providing navigation
             * options. Similar to main menu but with different action mappings for pause context.
             *
             * @param input Current input state for mouse interaction detection.
             * @param commands Vector of render commands to append menu elements to.
             * @return 1 for Resume action, 2 for Back to Main Menu action, 3 for Exit action, 0 for no action.
             */
            int updatePauseMenu(const Common::InputState &input, std::vector<Common::RenderCommand> &commands);

        private:
            /**
             * @brief Checks if a mouse click occurred within a button's boundaries.
             *
             * Performs collision detection between mouse coordinates and button rectangle
             * to determine if a button was clicked. Used by menu update methods to process
             * user interaction with menu buttons.
             *
             * @param mx Mouse X coordinate.
             * @param my Mouse Y coordinate.
             * @param bx Button X position.
             * @param by Button Y position.
             * @param bw Button width.
             * @param bh Button height.
             * @return True if mouse click is within button bounds.
             */
            bool isButtonClicked(int mx, int my, float bx, float by, float bw, float bh);
        };
    }
}
