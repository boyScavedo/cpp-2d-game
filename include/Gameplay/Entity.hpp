#pragma once

#include <vector>
#include <memory>
#include <optional>
#include "Common/Types.hpp"
#include "Gameplay/Components.hpp"
#include "Common/Constants.hpp"

namespace Gameplay
{
    /**
     * @class Entity
     * @brief Base class for all game objects, implementing the entity-component pattern.
     *
     * Entity represents any interactive or visual object in the game world, from players and enemies
     * to platforms and background elements. It uses composition over inheritance through optional components
     * (Transform, Sprite, Physics, etc.) to define behavior and appearance. This design allows flexible
     * entity creation without deep inheritance hierarchies. Entities are managed by EntityManager and
     * participate in the main game loop through update() and render() methods. The ID system enables
     * efficient lookup and referencing, while the destruction flag supports safe deferred removal.
     */
    class Entity
    {
    public:
        Entity() = default;          /**< Default constructor for basic entity initialization. */
        virtual ~Entity() = default; /**< Virtual destructor ensures proper cleanup of derived classes. */

        /**
         * @brief Updates the entity's state based on time and game logic.
         *
         * Called each frame by EntityManager. Implementations should handle movement, AI,
         * animation, and other time-based behaviors. Delta time ensures frame-rate independent updates.
         *
         * @param deltaTime Time elapsed since last update in seconds.
         */
        virtual void update(float deltaTime) = 0;

        /**
         * @brief Generates render commands for this entity.
         *
         * Populates the command vector with RenderCommand objects representing how this entity
         * should be drawn. Commands are processed by the Renderer in z-index order.
         *
         * @param commands Vector to append render commands to.
         */
        virtual void render(std::vector<Common::RenderCommand> &commands) = 0;

        Transform transform;                        /**< Spatial positioning and dimensions. */
        std::optional<Sprite> sprite;               /**< Visual representation via texture. */
        std::optional<Physics> physics;             /**< Physical properties and movement. */
        std::optional<Collider> collider;           /**< Collision detection boundaries. */
        std::optional<LevelExit> exit;              /**< Level transition trigger. */
        std::optional<PlayerControl> playerControl; /**< Player input handling. */
        std::optional<Parallax> parallax;           /**< Background scrolling behavior. */

        int id = -1;              /**< Unique identifier assigned by EntityManager. */
        bool isDestroyed = false; /**< Flag for deferred removal from the game world. */
    };

    /**
     * @class Player
     * @brief The player-controlled character entity with movement and interaction capabilities.
     *
     * Player represents the user's avatar in the game world, handling input processing, movement,
     * and player-specific behaviors. It combines multiple components (Transform, Sprite, Physics, etc.)
     * to create a responsive, controllable character. The class manages input translation into actions,
     * ensuring smooth platforming controls while integrating with the broader entity system.
     */
    class Player : public Entity
    {
    public:
        Player(); /**< Constructor initializes player with default components and settings. */

        /**
         * @brief Updates player state including physics and input processing.
         *
         * Handles player movement, jumping, collision responses, and other dynamic behaviors.
         * Called each frame as part of the entity update cycle.
         *
         * @param deltaTime Time elapsed since last update in seconds.
         */
        void update(float deltaTime) override;

        /**
         * @brief Generates render commands for the player character.
         *
         * Creates render commands to display the player sprite with proper positioning and layering.
         *
         * @param commands Vector to append player render commands to.
         */
        void render(std::vector<Common::RenderCommand> &commands) override;

        /**
         * @brief Processes input state to control player actions.
         *
         * Translates raw input (keyboard/mouse) into player movements and actions.
         * Handles directional movement, jumping, and other player controls.
         *
         * @param input Current input state from InputManager.
         */
        void handleInput(const Common::InputState &input);
    };

    /**
     * @class StaticObject
     * @brief Immutable environmental entities like platforms, walls, and decorations.
     *
     * StaticObject represents non-moving, non-interactive elements of the level geometry.
     * These entities provide the physical structure of the game world, including platforms
     * for standing, walls for boundaries, and decorative elements. They typically have
     * Transform, Sprite, and Collider components but no Physics (they don't move).
     * Static objects are essential for level design and collision detection.
     */
    class StaticObject : public Entity
    {
    public:
        StaticObject() = default; /**< Default constructor for static object initialization. */

        /**
         * @brief No-op update since static objects don't change over time.
         *
         * Static objects don't require updates as they maintain constant state.
         * The [[maybe_unused]] attribute suppresses compiler warnings.
         *
         * @param deltaTime Time elapsed (unused for static objects).
         */
        void update([[maybe_unused]] float deltaTime) override {}

        /**
         * @brief Generates render commands for the static object's appearance.
         *
         * Creates render commands to display the object's sprite at its fixed position.
         *
         * @param commands Vector to append static object render commands to.
         */
        void render(std::vector<Common::RenderCommand> &commands) override;
    };

    /**
     * @class BackgroundLayer
     * @brief Visual background elements with parallax scrolling for depth simulation.
     *
     * BackgroundLayer entities create the illusion of depth in 2D environments through
     * parallax scrolling. They move at different speeds relative to the camera, creating
     * layered backgrounds that enhance immersion. These entities typically have Transform,
     * Sprite, and Parallax components, but no collision (they're visual-only). Multiple
     * background layers can be combined for complex scenic effects.
     */
    class BackgroundLayer : public Entity
    {
    public:
        BackgroundLayer() = default; /**< Default constructor for background layer initialization. */

        /**
         * @brief No-op update since background layers are controlled by camera movement.
         *
         * Background position is determined by parallax calculations in the rendering system,
         * not by entity updates. The [[maybe_unused]] attribute suppresses compiler warnings.
         *
         * @param deltaTime Time elapsed (unused for background layers).
         */
        void update([[maybe_unused]] float deltaTime) override {}

        /**
         * @brief Generates render commands with parallax-adjusted positioning.
         *
         * Creates render commands that account for camera offset and parallax factor,
         * producing the scrolling background effect.
         *
         * @param commands Vector to append background render commands to.
         */
        void render(std::vector<Common::RenderCommand> &commands) override;
    };
}
