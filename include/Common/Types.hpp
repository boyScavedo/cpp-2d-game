#pragma once

#include "Common/Constants.hpp"

namespace Common
{
    /**
     * @enum GameState
     * @brief Defines the global states of the application, controlling the main game loop flow.
     *
     * This enumeration manages the high-level state machine of the game, determining what systems are active
     * (e.g., rendering, input processing, physics updates). Transitions between states handle menu navigation,
     * pausing, and game progression, ensuring a structured user experience from startup to exit.
     */
    enum class GameState
    {
        MAIN_MENU, /**< Displays the main menu UI, allowing game start, settings, or quit options. */
        RUNNING,   /**< Active gameplay state: processes input, updates entities, renders world. */
        PAUSED,    /**< Game is paused: shows pause menu, freezes entity updates but keeps rendering. */
        EXITING    /**< Application is shutting down: cleans up resources and terminates. */
    };

    /**
     * @struct InputState
     * @brief Represents the current state of user input, used to communicate from the Engine to Gameplay modules.
     *
     * This struct aggregates all possible input sources (keyboard, mouse) into a unified state object.
     * It decouples input polling from gameplay logic, allowing the Engine to handle device specifics
     * while Gameplay focuses on responding to actions. Boolean flags enable responsive controls,
     * and mouse coordinates support UI interactions and editor tools.
     */
    struct InputState
    {
        // Movement and Action Keys
        bool up = false;               /**< True if the up movement key (W) is pressed; triggers upward actions. */
        bool down = false;             /**< True if the down movement key (S) is pressed; for crouching or downward movement. */
        bool left = false;             /**< True if the left movement key (A) is pressed; moves player left. */
        bool right = false;            /**< True if the right movement key (D) is pressed; moves player right. */
        bool jump = false;             /**< True if the jump key (Space) is pressed; initiates player jump. */
        bool attack = false;           /**< True if the attack key/button (left mouse) is pressed; triggers combat. */
        bool toggleFullScreen = false; /**< True if the fullscreen toggle key (F11) is pressed; switches display mode. */
        bool quit = false;             /**< True if the quit event (window close, ESC) has been triggered; signals exit. */

        // Arrow Keys (Separate from WASD for UI navigation)
        bool arrowUp = false;    /**< True if up arrow key is pressed; used for menu navigation. */
        bool arrowDown = false;  /**< True if down arrow key is pressed; for menu selection. */
        bool arrowLeft = false;  /**< True if left arrow key is pressed; for horizontal menu movement. */
        bool arrowRight = false; /**< True if right arrow key is pressed; for horizontal menu movement. */

        // Editor Keys (for level editing mode)
        bool c = false;         /**< True if 'C' key is pressed; creates new entities in editor. */
        bool backspace = false; /**< True if Backspace is pressed; deletes selected entities. */
        bool enter = false;     /**< True if Enter is pressed; confirms actions or submits forms. */
        bool shift = false;     /**< True if Shift is held; modifies actions (e.g., multi-select). */
        bool ctrl = false;      /**< True if Ctrl is held; enables shortcuts (e.g., save, copy). */
        bool pause = false;     /**< True if 'P' or Escape is pressed; toggles pause state. */

        // Mouse Input (for UI and editor interactions)
        int mouseX = 0;             /**< Current mouse X position in screen coordinates. */
        int mouseY = 0;             /**< Current mouse Y position in screen coordinates. */
        bool mouseLeftDown = false; /**< True if left mouse button is currently pressed. */
    };

    /**
     * @struct RenderCommand
     * @brief Represents a command to render a texture at a specific position and size, used to communicate from Gameplay to Engine modules.
     *
     * This struct encapsulates all data needed for a single rendering operation, enabling batched drawing.
     * It supports both textured sprites (via textureID) and solid colors (when textureID is TEX_NONE),
     * with parallax scrolling controlled by scrollFactor. This design separates rendering logic from
     * gameplay updates, allowing the Renderer to optimize draw calls and handle camera transformations.
     */
    struct RenderCommand
    {
        float x = 0.0f, y = 0.0f;                                  /**< The x and y coordinates of the render position in world space. */
        float width = 0.0f, height = 0.0f;                         /**< The width and height of the rendered element in pixels. */
        Common::TextureID textureID = Common::TextureID::TEX_NONE; /**< The ID of the texture to render; TEX_NONE for solid colors. */
        float scrollFactor = 1.0f;                                 /**< The scroll factor for parallax effects (0.0 = fixed UI, 1.0 = full camera scroll). */

        // Custom Color (used when textureID == TEX_NONE for overlays or UI elements)
        unsigned char colorR = 0, colorG = 255, colorB = 255, colorA = 255; /**< RGBA color values for solid-color rendering. */
    };
}
