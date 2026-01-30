#pragma once

#include "Common/Constants.hpp"

namespace Common
{
    /**
     * @struct InputState
     * @brief Represents the current state of user input, used to communicate from the Engine to Gameplay modules.
     */
    struct InputState
    {
        bool up = false;               /**< True if the up movement key is pressed. */
        bool down = false;             /**< True if the down movement key is pressed. */
        bool left = false;             /**< True if the left movement key is pressed. */
        bool right = false;            /**< True if the right movement key is pressed. */
        bool jump = false;             /**< True if the jump key is pressed. */
        bool attack = false;           /**< True if the attack key/button is pressed. */
        bool toggleFullScreen = false; /**< True if the fullscreen toggle key is pressed. */
        bool quit = false;             /**< True if the quit event has been triggered. */
    };

    /**
     * @struct RenderCommand
     * @brief Represents a command to render a texture at a specific position and size, used to communicate from Gameplay to Engine modules.
     */
    struct RenderCommand
    {
        float x = 0.0f, y = 0.0f;                                   /**< The x and y coordinates of the render position. */
        float width = 0.0f, height = 0.0f;                          /**< The width and height of the rendered element. */
        Common::TextureID textureID = Common::TextureID::TEXT_NONE; /**< The ID of the texture to render. */
        float scrollFactor = 1.0f;                                  /**< The scroll factor for parallax effects (0.0 = fixed, 1.0 = full camera scroll). */
    };
}
