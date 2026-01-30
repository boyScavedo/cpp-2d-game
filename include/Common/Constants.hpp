#pragma once

namespace Common
{
    // --- Window Settings ---
    inline constexpr int SCREEN_WIDTH = 1280;
    inline constexpr int SCREEN_HEIGHT = 720;
    inline constexpr int MINIMUM_SCREEN_WIDTH = 854;
    inline constexpr int MINIMUM_SCREEN_HEIGHT = 480;
    inline constexpr const char *WINDOW_TITLE = "2D Roguelike-Metroidvania v0.0.1";

    // --- Physics & Gameplay Logic ---
    inline constexpr float TARGET_FPS = 60.0f;
    inline constexpr float TIME_STEP = 1.0f / TARGET_FPS;

    // --- Texture & Sprite IDs ---
    enum class TextureID : int
    {
        TEX_PLAYER = 0,
        TEX_WALL = 1,
        TEX_FLOOR = 2,
        TEX_ENEMY = 3,
        TEX_COUNT,
        TEXT_NONE = -1
    };

    // --- Tile/Grid Settings ---
    inline constexpr int TILE_SIZE = 32;
}