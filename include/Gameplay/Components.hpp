#pragma once

#include "Common/Constants.hpp"
#include <string>

namespace Gameplay
{
    /**
     * @struct Transform
     * @brief Represents the position, scale, and layering of an entity in the world.
     */
    struct Transform
    {
        float x = 0.0f;
        float y = 0.0f;
        float width = 32.0f;
        float height = 32.0f;
        int zIndex = 0;
    };

    /**
     * @struct Sprite
     * @brief Defines the visual representation of an entity using a texture.
     */
    struct Sprite
    {
        Common::TextureID textureID = Common::TextureID::TEX_NONE;
        int zIndex = 0;
    };

    /**
     * @struct Physics
     * @brief Contains physical properties and state for movement and collision resolution.
     */
    struct Physics
    {
        float velocityX = 0.0f;
        float velocityY = 0.0f;
        
        float gravity = Common::GRAVITY;
        float friction = Common::FRICTION;
        float maxSpeed = Common::PLAYER_MAX_SPEED;
        bool isGrounded = false;
    };

    /**
     * @enum CardinalDirection
     * @brief Represents the four primary compass directions for spawns and transitions.
     */
    enum class CardinalDirection { North, South, East, West };

    /**
     * @struct Collider
     * @brief Defines a rectangular hitbox for collision detection.
     */
    struct Collider
    {
        float width = 32.0f;
        float height = 32.0f;
        float offsetX = 0.0f;
        float offsetY = 0.0f;
        bool isTrigger = false;
        bool isSolid = false;
    };

    /**
     * @struct LevelExit
     * @brief Component used to trigger transitions to other levels.
     */
    struct LevelExit
    {
        CardinalDirection direction = CardinalDirection::East;
        std::string nextLevelPath;
        float transitionDuration = 0.5f;
    };

    /**
     * @struct PlayerControl
     * @brief Data for player-controlled entities.
     */
    struct PlayerControl
    {
        float speed = 200.0f;
        bool canJump = true;
    };

    /**
     * @struct Parallax
     * @brief Defines parallax scrolling behavior for background layers.
     */
    struct Parallax
    {
        float factor = 1.0f;
    };
}
