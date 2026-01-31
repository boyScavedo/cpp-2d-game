#pragma once

/**
 * @namespace Common
 * @brief Contains common constants, types, and utilities shared across the game engine and gameplay modules.
 */
namespace Common
{
    // --- Window Settings ---
    /** @brief The width of the game screen in pixels. */
    inline constexpr int SCREEN_WIDTH = 1280;
    /** @brief The height of the game screen in pixels. */
    inline constexpr int SCREEN_HEIGHT = 720;
    /** @brief The minimum allowed width for the game window in pixels. */
    inline constexpr int MINIMUM_SCREEN_WIDTH = 854;
    /** @brief The minimum allowed height for the game window in pixels. */
    inline constexpr int MINIMUM_SCREEN_HEIGHT = 480;
    /** @brief The prefix for the window title, including version and FPS display. */
    inline constexpr const char *WINDOW_TITLE_PREFIX = "2D Roguelike-Metroidvania v0.0.1 | FPS: ";

    // --- Camera Settings ---
    /** @brief The width of the camera viewport, matching the screen width. */
    inline constexpr int VIEWPORT_WIDTH = SCREEN_WIDTH;
    /** @brief The total width of the game world for sidescrolling. */
    inline constexpr int WORLD_WIDTH = 2560; // Larger world for sidescrolling
    /** @brief The minimum allowed camera offset on the X-axis. */
    inline constexpr float MINIMUM_CAMERA_OFFSET_X = 0.0f;
    /** @brief The maximum allowed camera offset on the X-axis, based on world and viewport size. */
    inline constexpr float MAXIMUM_CAMERA_OFFSET_X = WORLD_WIDTH - VIEWPORT_WIDTH;

    // --- Player Settings ---
    /** @brief The width of the player character in pixels. */
    inline constexpr float PLAYER_WIDTH = 50.0f;
    /** @brief The height of the player character in pixels. */
    inline constexpr float PLAYER_HEIGHT = 50.0f;

    // --- Physics & Gameplay Logic ---
    /** @brief The target frames per second for the game loop. */
    inline constexpr float TARGET_FPS = 60.0f;
    /** @brief The fixed time step per frame, derived from target FPS. */
    inline constexpr float TIME_STEP = 1.0f / TARGET_FPS;
    /** @brief The gravitational acceleration applied to entities. */
    inline constexpr float GRAVITY = 980.0f;
    /** @brief The maximum downward velocity an entity can reach. */
    inline constexpr float TERMINAL_VELOCITY = 5399.8f;
    /** @brief The acceleration applied to the player for horizontal movement. */
    inline constexpr float PLAYER_ACCELERATION = 5000.0f;
    /** @brief The friction coefficient for slowing down horizontal movement. */
    inline constexpr float FRICTION = 10000.0f;
    /** @brief The upward force applied when the player jumps. */
    inline constexpr float JUMP_FORCE = -500.0f;
    /** @brief The maximum horizontal speed the player can reach. */
    inline constexpr float PLAYER_MAX_SPEED = 500.0f;

    // --- Texture & Sprite IDs ---
    /**
     * @enum TextureID
     * @brief Enumeration of texture identifiers used for rendering different game elements.
     */
    enum class TextureID : int
    {
        TEX_PLAYER = 0,          /**< Texture for the player character. */
        TEX_WALL = 1,            /**< Texture for wall tiles. */
        TEX_FLOOR = 2,           /**< Texture for floor tiles. */
        TEX_ENEMY = 3,           /**< Texture for enemy characters. */
        TEX_BACKGROUND_FAR = 4,  /**< Texture for the far background layer. */
        TEX_BACKGROUND_MID = 5,  /**< Texture for the mid background layer. */
        TEX_BACKGROUND_NEAR = 6, /**< Texture for the near background layer. */
        TEX_COUNT,               /**< Total number of textures; used for array sizing. */
        TEXT_NONE = -1           /**< Special value indicating no texture. */
    };

    // --- Tile/Grid Settings ---
    /** @brief The size of each tile in the game grid, in pixels. */
    inline constexpr int TILE_SIZE = 32;
}