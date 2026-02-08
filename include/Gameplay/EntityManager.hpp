#pragma once

#include <vector>
#include <memory>
#include <string>
#include "Common/Types.hpp"
#include "Gameplay/Entity.hpp"
#include "Common/Constants.hpp"

namespace Gameplay
{
    /**
     * @struct LevelConfig
     * @brief Configuration settings for the current level's physical boundaries and behavior.
     *
     * Defines level-specific properties that affect gameplay mechanics, such as boundary enforcement
     * for player movement and level dimensions used in camera calculations and collision detection.
     * These settings allow levels to have different physical properties (e.g., open worlds vs. confined spaces).
     */
    struct LevelConfig
    {
        bool isLeftWallClamped = true;  /**< Whether the left level boundary blocks player movement. */
        bool isRightWallClamped = true; /**< Whether the right level boundary blocks player movement. */
        float levelWidth = 2560.0f;     /**< Total width of the level in pixels, affecting camera limits. */
    };

    /**
     * @class EntityManager
     * @brief Central coordinator for all game entities, managing their lifecycle, updates, and interactions.
     *
     * EntityManager implements an entity-component system, maintaining a collection of all active entities
     * in the current level. It orchestrates the update loop (physics, input processing), rendering pipeline,
     * and collision detection. The manager provides methods for adding/removing entities, querying specific
     * entities (like the player), and serializing the entire level state for saving/loading. This class
     * acts as the bridge between individual entities and the broader game systems (Renderer, InputManager, etc.),
     * ensuring cohesive gameplay behavior across all objects in the scene.
     */
    class EntityManager
    {
    public:
        EntityManager() = default; /**< Default constructor initializes an empty entity manager. */

        /**
         * @brief Updates all managed entities with the current time delta and input state.
         *
         * Iterates through all entities, calling their update methods. Handles player input processing,
         * physics simulation, and entity state changes. Also removes any entities marked for destruction.
         * This method is called once per frame by the main game loop.
         *
         * @param deltaTime Time elapsed since the last update in seconds.
         * @param input Current input state for processing player controls.
         */
        void update(float deltaTime, const Common::InputState &input);

        /**
         * @brief Collects render commands from all visible entities for the Renderer.
         *
         * Iterates through entities, calling their render methods to generate RenderCommand objects.
         * Commands are collected in the provided vector, which is then passed to the Renderer for drawing.
         * Entities are processed in order to ensure proper z-index layering.
         *
         * @param commands Vector to append render commands to, sorted by z-index.
         */
        void render(std::vector<Common::RenderCommand> &commands);

        /**
         * @brief Adds a new entity to the manager's collection.
         *
         * Assigns a unique ID to the entity and stores it in the internal collection.
         * The entity will participate in future update and render cycles.
         *
         * @param entity Shared pointer to the entity to add (ownership is shared).
         */
        void addEntity(std::shared_ptr<Entity> entity);

        /**
         * @brief Removes a specific entity from the manager.
         *
         * Finds and removes the entity from the collection by pointer comparison.
         * The entity will no longer be updated or rendered.
         *
         * @param entity Shared pointer to the entity to remove.
         */
        void removeEntity(std::shared_ptr<Entity> entity);

        /**
         * @brief Removes an entity by its unique ID.
         *
         * Searches for the entity with the matching ID and removes it from the collection.
         * Useful for deferred removal or when only the ID is known.
         *
         * @param entityID The unique identifier of the entity to remove.
         */
        void removeEntity(int entityID);

        /**
         * @brief Removes all entities from the manager.
         *
         * Clears the entire entity collection, typically used when loading a new level
         * or resetting the game state. All entity references become invalid.
         */
        void clear();

        /**
         * @brief Retrieves the player entity if present in the level.
         *
         * Searches the entity collection for an entity with PlayerControl component.
         * Assumes at most one player entity exists in a level.
         *
         * @return Shared pointer to the player entity, or nullptr if no player found.
         */
        std::shared_ptr<Player> getPlayer() const;

        /**
         * @brief Provides read-only access to the full entity collection.
         *
         * Returns a const reference to the internal entity vector, allowing iteration
         * or inspection without modification. Useful for debugging or external systems.
         *
         * @return Const reference to the vector of all managed entities.
         */
        const std::vector<std::shared_ptr<Entity>> &getEntities() const { return m_entities; }

        /**
         * @brief Checks for collision-based interactions, particularly level exits.
         *
         * Performs collision detection between entities to identify triggered level exits.
         * Returns the LevelExit component data if the player has entered an exit zone,
         * enabling level transitions in the main game loop.
         *
         * @return Optional LevelExit data if a transition is triggered, empty otherwise.
         */
        std::optional<LevelExit> checkCollisions();

        /**
         * @brief Serializes the current level state to JSON format.
         *
         * Converts all entities, their components, and level configuration to a JSON string.
         * Used by the level editor for saving custom levels to disk.
         *
         * @return JSON string representation of the entire level state.
         */
        std::string toJSON() const;

        LevelConfig levelConfig; /**< Configuration for the current level's physical properties. */

    private:
        std::vector<std::shared_ptr<Entity>> m_entities; /**< Collection of all active entities in the level. */
        int m_nextEntityID = 0;                          /**< Counter for assigning unique IDs to new entities. */
    };
}
