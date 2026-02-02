#pragma once
#include <SDL3/SDL.h>
#include "Common/Types.hpp"

namespace Engine
{
    /**
     * @class InputManager
     * @brief Manages user input from keyboard and mouse, updating the input state each frame.
     *
     * The InputManager polls SDL events and keyboard/mouse states to update an InputState struct,
     * which is used by other systems to respond to user actions.
     */
    class InputManager
    {
    private:
        Common::InputState m_state{}; /**< The current state of all input flags. */

    public:
        /**
         * @brief Polls system input and updates the stored input state.
         *
         * Updates directional and action flags and records high-level input events:
         * - Sets `quit` when a quit event is received.
         * - Sets `toggleFullScreen` when the F11 key is pressed.
         * - Updates `up`, `down`, `left`, `right` from the W/S/A/D keys.
         * - Updates `jump` from the Space key.
         * - Updates `attack` from the left mouse button.
         *
         * @return Common::InputState The updated input state reflecting the current values of
         * `up`, `down`, `left`, `right`, `jump`, `attack`, `toggleFullScreen`, and `quit`.
         */
        Common::InputState update();
    };
}