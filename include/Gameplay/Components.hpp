#pragma once

#include "Common/Constants.hpp"
#include <string>

namespace Gameplay
{
    /**
     * @struct Transform
     * @brief Represents the spatial positioning, dimensions, and rendering order of an entity in the game world.
     *
     * The Transform component is fundamental to all entities, defining where and how large they appear in 2D space.
     * It supports the game's coordinate system, enabling precise placement for gameplay mechanics like collision
     * detection and rendering. The zIndex controls draw order, allowing layered visuals (backgrounds behind foregrounds).
     * This component integrates with the Camera and Renderer systems for world-to-screen transformations.
     */
    struct Transform
    {
        float x = 0.0f;       /**< World-space X coordinate (horizontal position). */
        float y = 0.0f;       /**< World-space Y coordinate (vertical position). */
        float width = 32.0f;  /**< Entity width in pixels (affects rendering and collision). */
        float height = 32.0f; /**< Entity height in pixels (affects rendering and collision). */
        int zIndex = 0;       /**< Rendering layer; higher values draw on top of lower ones. */
    };

    /**
     * @struct Sprite
     * @brief Defines the visual appearance of an entity through texture mapping and layering.
     *
     * The Sprite component links entities to their graphical representation, using pre-loaded textures
     * from the Renderer. It supports the game's visual style by associating entities with specific
     * images (player, enemies, tiles). The zIndex complements Transform's layering for complex
     * visual hierarchies, enabling effects like foreground details over backgrounds.
     */
    struct Sprite
    {
        Common::TextureID textureID = Common::TextureID::TEX_NONE; /**< ID of the texture to render for this entity. */
        int zIndex = 0;                                            /**< Additional layering control for sprite rendering. */
    };

    /**
     * @struct Physics
     * @brief Encapsulates physical simulation properties for realistic movement and environmental interaction.
     *
     * The Physics component enables dynamic entity behavior through velocity, gravity, and friction.
     * It powers the game's platforming mechanics, allowing entities to fall, accelerate, and decelerate
     * naturally. Grounded state prevents multiple jumps and enables stable movement. This component
     * works with the EntityManager's update loop to simulate physics each frame, creating responsive
     * and predictable gameplay.
     */
    struct Physics
    {
        float velocityX = 0.0f; /**< Current horizontal velocity (pixels per second). */
        float velocityY = 0.0f; /**< Current vertical velocity (pixels per second). */

        float gravity = Common::GRAVITY;           /**< Downward acceleration applied each frame. */
        float friction = Common::FRICTION;         /**< Deceleration factor for horizontal movement. */
        float maxSpeed = Common::PLAYER_MAX_SPEED; /**< Maximum allowed horizontal speed. */
        bool isGrounded = false;                   /**< True when entity is on a surface, enabling jumps. */
    };

    /**
     * @enum CardinalDirection
     * @brief Defines the four cardinal directions for level transitions and entity spawning.
     *
     * This enumeration standardizes directional logic throughout the game, used for seamless
     * level connections and spawn point calculations. It ensures consistent behavior when
     * moving between areas or respawning entities at level edges.
     */
    enum class CardinalDirection
    {
        North, /**< Upward direction (positive Y in some systems, but context-dependent). */
        South, /**< Downward direction (negative Y). */
        East,  /**< Rightward direction (positive X). */
        West   /**< Leftward direction (negative X). */
    };

    /**
     * @struct Collider
     * @brief Defines a rectangular collision boundary for interaction detection and solid object behavior.
     *
     * The Collider component enables the game's physics interactions by defining hitboxes that detect
     * overlaps with other entities. It supports both solid collisions (blocking movement) and trigger
     * zones (activating events). Offset values allow fine-tuning hitbox position relative to the entity's
     * visual bounds, crucial for accurate gameplay feel in platformers.
     */
    struct Collider
    {
        float width = 32.0f;    /**< Width of the collision rectangle. */
        float height = 32.0f;   /**< Height of the collision rectangle. */
        float offsetX = 0.0f;   /**< Horizontal offset from entity's transform position. */
        float offsetY = 0.0f;   /**< Vertical offset from entity's transform position. */
        bool isTrigger = false; /**< If true, detects overlaps but doesn't block movement. */
        bool isSolid = false;   /**< If true, prevents other entities from passing through. */
    };

    /**
     * @struct LevelExit
     * @brief Marks an entity as a portal for transitioning between game levels.
     *
     * The LevelExit component creates interactive zones that load new levels when triggered by the player.
     * It supports the game's world-building by enabling connected level designs, with smooth transitions
     * controlled by duration. Direction ensures proper spawn positioning in the new level, maintaining
     * seamless exploration experiences.
     */
    struct LevelExit
    {
        CardinalDirection direction = CardinalDirection::East; /**< Direction of exit for spawn calculation. */
        std::string nextLevelPath;                             /**< File path to the level to load on trigger. */
        float transitionDuration = 0.5f;                       /**< Time in seconds for fade transition. */
    };

    /**
     * @struct PlayerControl
     * @brief Contains configuration for player-controlled entity behavior and abilities.
     *
     * This component defines the parameters for entities under direct player input, controlling
     * movement speed and jump capabilities. It allows customization of player feel and enables
     * future features like power-ups that modify these values. The component integrates with
     * InputManager and Physics to translate player actions into entity movement.
     */
    struct PlayerControl
    {
        float speed = 200.0f; /**< Base movement speed for horizontal input. */
        bool canJump = true;  /**< Whether the player can initiate jumps. */
    };

    /**
     * @struct Parallax
     * @brief Controls background layer movement for depth simulation in 2D environments.
     *
     * The Parallax component creates visual depth by moving background elements at different speeds
     * relative to the camera. A factor of 1.0 matches camera movement (fixed), while lower values
     * create slower movement for distant layers. This enhances immersion in side-scrolling games
     * by simulating 3D depth through layered 2D graphics.
     */
    struct Parallax
    {
        float factor = 1.0f; /**< Parallax scroll factor (0.0 = static, 1.0 = camera speed). */
    };
}
