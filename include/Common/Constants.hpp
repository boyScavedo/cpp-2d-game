#pragma once

/**
 * @namespace Common
 * @brief Contains common constants, types, and utilities shared across the game engine and gameplay modules.
 *
 * This namespace centralizes configuration values that define the game's visual, physical, and logical parameters.
 * These constants ensure consistency across rendering, physics, input handling, and gameplay systems, making the
 * game easily configurable and maintainable. For example, adjusting SCREEN_WIDTH affects the viewport, while
 * GRAVITY influences all entities' falling behavior, promoting a cohesive game world.
 */
namespace Common
{
    // --- Window Settings ---
    /**
     * @brief The width of the game screen in pixels.
     * @details Defines the resolution for rendering and UI elements. Affects camera viewport and overall display.
     */
    inline constexpr int SCREEN_WIDTH = 1280;
    /**
     * @brief The height of the game screen in pixels.
     * @details Sets the vertical resolution, impacting UI layout and gameplay area.
     */
    inline constexpr int SCREEN_HEIGHT = 720;
    /**
     * @brief The minimum allowed width for the game window in pixels.
     * @details Ensures the game remains playable on smaller screens by preventing window resizing below this threshold.
     */
    inline constexpr int MINIMUM_SCREEN_WIDTH = 854;
    /**
     * @brief The minimum allowed height for the game window in pixels.
     * @details Maintains usability by restricting minimum window height.
     */
    inline constexpr int MINIMUM_SCREEN_HEIGHT = 480;
    /**
     * @brief The prefix for the window title, including version and FPS display.
     * @details Used to construct the dynamic window title, showing real-time FPS for performance monitoring.
     */
    inline constexpr const char *WINDOW_TITLE_PREFIX = "2D Roguelike-Metroidvania v0.0.1 | FPS: ";

    // --- Camera Settings ---
    /**
     * @brief The width of the camera viewport, matching the screen width.
     * @details Determines the visible horizontal area, synchronized with SCREEN_WIDTH for seamless rendering.
     */
    inline constexpr int VIEWPORT_WIDTH = SCREEN_WIDTH;
    /**
     * @brief The total width of the game world for sidescrolling.
     * @details Defines the expansive world size, allowing for exploration beyond the initial screen.
     */
    inline constexpr int WORLD_WIDTH = 2560; // Larger world for sidescrolling
    /**
     * @brief The minimum allowed camera offset on the X-axis.
     * @details Prevents the camera from scrolling left beyond the world start, maintaining boundaries.
     */
    inline constexpr float MINIMUM_CAMERA_OFFSET_X = 0.0f;
    /**
     * @brief The maximum allowed camera offset on the X-axis, based on world and viewport size.
     * @details Ensures the camera doesn't scroll past the world's end, calculated as WORLD_WIDTH - VIEWPORT_WIDTH.
     */
    inline constexpr float MAXIMUM_CAMERA_OFFSET_X = WORLD_WIDTH - VIEWPORT_WIDTH;

    // --- Player Settings ---
    /**
     * @brief The width of the player character in pixels.
     * @details Affects collision detection and rendering size for the player entity.
     */
    inline constexpr float PLAYER_WIDTH = 50.0f;
    /**
     * @brief The height of the player character in pixels.
     * @details Determines player sprite dimensions and hitbox height.
     */
    inline constexpr float PLAYER_HEIGHT = 50.0f;

    /**
     * @brief The starting position of the game world for sidescrolling.
     * @details Sets the initial world coordinate, often negative to allow player spawn without immediate boundary issues.
     */
    inline constexpr float WORLD_START = -PLAYER_WIDTH;

    // --- Physics & Gameplay Logic ---
    /**
     * @brief The target frames per second for the game loop.
     * @details Ensures smooth gameplay by aiming for consistent frame timing; affects deltaTime calculations.
     */
    inline constexpr float TARGET_FPS = 60.0f;
    /**
     * @brief The fixed time step per frame, derived from target FPS.
     * @details Used in physics updates to maintain deterministic behavior regardless of frame rate.
     */
    inline constexpr float TIME_STEP = 1.0f / TARGET_FPS;
    /**
     * @brief The gravitational acceleration applied to entities.
     * @details Simulates realistic falling; higher values increase pull toward the ground.
     */
    inline constexpr float GRAVITY = 980.0f;
    /**
     * @brief The maximum downward velocity an entity can reach.
     * @details Prevents infinite falling speed, capping terminal velocity for balanced physics.
     */
    inline constexpr float TERMINAL_VELOCITY = 5399.8f;
    /**
     * @brief The acceleration applied to the player for horizontal movement.
     * @details Controls how quickly the player speeds up when moving left/right.
     */
    inline constexpr float PLAYER_ACCELERATION = 5000.0f;
    /**
     * @brief The friction coefficient for slowing down horizontal movement.
     * @details Applies deceleration when no input is given, simulating surface resistance.
     */
    inline constexpr float FRICTION = 10000.0f;
    /**
     * @brief The upward force applied when the player jumps.
     * @details Negative value propels the player upward; magnitude affects jump height.
     */
    inline constexpr float JUMP_FORCE = -500.0f;
    /**
     * @brief The maximum horizontal speed the player can reach.
     * @details Caps player velocity for control and balance in platforming.
     */
    inline constexpr float PLAYER_MAX_SPEED = 500.0f;

    // --- Texture & Sprite IDs ---
    /**
     * @enum TextureID
     * @brief Enumeration of texture identifiers used for rendering different game elements.
     *
     * This enum maps game assets to unique IDs, facilitating efficient texture management and rendering.
     * Background layers support parallax scrolling, while entity textures enable dynamic sprite display.
     */
    enum class TextureID : int
    {
        TEX_PLAYER = 0,          /**< Texture for the player character; central to gameplay interactions. */
        TEX_WALL = 1,            /**< Texture for wall tiles; used in level design for obstacles. */
        TEX_FLOOR = 2,           /**< Texture for floor tiles; forms the walkable surface. */
        TEX_ENEMY = 3,           /**< Texture for enemy characters; represents threats in the game world. */
        TEX_BACKGROUND_FAR = 4,  /**< Texture for the far background layer; slowest parallax for depth. */
        TEX_BACKGROUND_MID = 5,  /**< Texture for the mid background layer; medium parallax speed. */
        TEX_BACKGROUND_NEAR = 6, /**< Texture for the near background layer; faster parallax for immersion. */
        TEX_COUNT,               /**< Total number of textures; used for array sizing and iteration. */
        TEX_NONE = -1            /**< Special value indicating no texture; for overlays or solid colors. */
    };

    // --- Tile/Grid Settings ---
    /**
     * @brief The size of each tile in the game grid, in pixels.
     * @details Standardizes level construction; affects collision grids and rendering alignment.
     */
    inline constexpr int TILE_SIZE = 32;

    // --- UI/Menu Settings ---
    /**
     * @brief The width of menu buttons in pixels.
     * @details Used for start and exit buttons in main menu, and resume/main menu/exit in pause menu.
     */
    inline constexpr float MENU_BUTTON_WIDTH = 250.0f;
    /**
     * @brief The height of menu buttons in pixels.
     * @details Standard height for menu buttons.
     */
    inline constexpr float MENU_BUTTON_HEIGHT = 80.0f;
    /**
     * @brief The width of the pause menu panel in pixels.
     * @details Defines the centered panel size in pause menu.
     */
    inline constexpr float PAUSE_PANEL_WIDTH = 350.0f;
    /**
     * @brief The height of the pause menu panel in pixels.
     * @details Defines the vertical size of the pause panel.
     */
    inline constexpr float PAUSE_PANEL_HEIGHT = 300.0f;

    // --- Level Editor Settings ---
    /**
     * @brief The movement speed for level editor camera and objects, in pixels per second.
     * @details Controls how fast the editor moves when panning or manipulating objects.
     */
    inline constexpr float LEVEL_EDITOR_MOVE_SPEED = 400.0f;
    /**
     * @brief The scaling speed for level editor objects, in pixels per second.
     * @details Controls how fast objects grow/shrink when resizing.
     */
    inline constexpr float LEVEL_EDITOR_SCALE_SPEED = 200.0f;
    /**
     * @brief The default X position for newly created objects in the level editor.
     * @details Starting position for object creation.
     */
    inline constexpr float DEFAULT_OBJECT_X = 400.0f;
    /**
     * @brief The default Y position for newly created objects in the level editor.
     * @details Starting position for object creation.
     */
    inline constexpr float DEFAULT_OBJECT_Y = 300.0f;

}
