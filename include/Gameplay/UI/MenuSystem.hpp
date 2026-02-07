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
         * @brief Handles rendering and interaction for Main Menu and Pause Menu.
         */
        class MenuSystem
        {
        public:
            MenuSystem() = default;

            /**
             * @brief Processes input and renders the Main Menu.
             * @param input Current input state.
             * @param commands Vector of render commands to append to.
             * @return 1 for Start Game, 2 for Exit, 0 for nothing.
             */
            int updateMainMenu(const Common::InputState& input, std::vector<Common::RenderCommand>& commands);

            /**
             * @brief Processes input and renders the Pause Menu.
             * @param input Current input state.
             * @param commands Vector of render commands to append to.
             * @return 1 for Resume, 2 for Back to Main Menu, 0 for nothing.
             */
            int updatePauseMenu(const Common::InputState& input, std::vector<Common::RenderCommand>& commands);

        private:
            bool isButtonClicked(int mx, int my, float bx, float by, float bw, float bh);
        };
    }
}
