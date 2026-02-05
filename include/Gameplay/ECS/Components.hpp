#pragma once

#include "Common/Constants.hpp"
#include <string>

namespace Gameplay
{
    namespace ECS
    {
        // 1. Position & Scale
        struct Transform
        {
            float x = 0.0f;
            float y = 0.0f;
            float width = 32.0f;
            float height = 32.0f;
        };

        // 2. Visuals
        struct Sprite
        {
            Common::TextureID textureID = Common::TextureID::TEX_NONE;
            int zIndex = 0; // 0=far bg, 10=player, etc.
        };

        // 3. Movement Physics
        struct Physics
        {
            float velocityX = 0.0f;
            float velocityY = 0.0f;
            
            // Physics properties
            float gravity = Common::GRAVITY;
            float friction = Common::FRICTION;
            float maxSpeed = Common::PLAYER_MAX_SPEED;
            bool isGrounded = false;
        };

        // 4. Input Tag (Identifies the player)
        struct PlayerControl
        {
            float speed = 200.0f; // Could be modifier
            bool canJump = true;
        };
        
        // 5. Background Tag (Identifies parallax layers)
        struct Parallax
        {
            float factor = 1.0f; // 1.0 = normal, 0.5 = half speed
        };

        // 6. Cardinal Directions
        enum class CardinalDirection { North, South, East, West };

        // 7. Generic Hitbox
        struct Collider
        {
            float width = 32.0f;
            float height = 32.0f;
            float offsetX = 0.0f;
            float offsetY = 0.0f;
            bool isTrigger = false; // If true, collision triggers event but doesn't block physics
        };

        // 8. Level Exit Trigger
        struct LevelExit
        {
            CardinalDirection direction = CardinalDirection::East;
            std::string nextLevelPath;
            float transitionDuration = 0.5f;
        };

    } // namespace ECS
} // namespace Gameplay
