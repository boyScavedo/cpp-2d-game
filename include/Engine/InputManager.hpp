#pragma once
#include <SDL3/SDL.h>
#include "Common/Types.hpp"

namespace Engine
{
    /**
     * @class InputManager
     * @brief Manages user input from keyboard and mouse, updating the input state each frame.
     *
     * The InputManager serves as the bridge between SDL's low-level input handling and the game's
     * high-level input requirements. It abstracts platform-specific input details, providing a
     * unified InputState interface for gameplay systems. This decoupling allows the game logic
     * to focus on responding to actions rather than polling hardware states. The manager supports
     * both windowed and fullscreen input, with mouse coordinate conversion for UI interactions.
     * It integrates with the main game loop, ensuring responsive controls for movement, actions,
     * and system commands like fullscreen toggling.
     */
    class InputManager
    {
    private:
        Common::InputState m_state{}; /**< The current state of all input flags, updated each frame. */

    public:
        /**
         * @brief Polls system input and updates the stored input state.
         *
         * Processes SDL events and keyboard states to update the internal InputState.
         * Handles directional movement (WASD), actions (Space for jump, mouse for attack),
         * system commands (F11 for fullscreen, ESC for quit), and editor keys for level editing.
         * This method is called each frame to capture the latest input state.
         *
         * @return Common::InputState The updated input state reflecting current key/mouse states.
         */
        Common::InputState update();

        /**
         * @brief Polls system input with mouse coordinates converted to logical renderer space.
         *
         * Similar to update(), but converts mouse coordinates from screen space to renderer logical space.
         * This is crucial for UI elements and editor interactions that need accurate positioning
         * regardless of window scaling or fullscreen mode. The renderer parameter provides the
         * coordinate transformation context.
         *
         * @param renderer The SDL_Renderer used for converting mouse coordinates to logical space.
         * @return Common::InputState The updated input state with converted mouse coordinates.
         */
        Common::InputState update(SDL_Renderer *renderer);
    };
}
