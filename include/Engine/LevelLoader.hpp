#pragma once

#include <string>
#include "Gameplay/EntityManager.hpp"

namespace Engine
{
    /**
     * @class LevelLoader
     * @brief Handles loading and deserializing level data from JSON files into the game world.
     *
     * The LevelLoader is responsible for bridging the gap between static level design (stored as JSON)
     * and the dynamic game world (managed by EntityManager). It parses level files containing entity
     * definitions, level configuration, and world layout, then instantiates the corresponding game objects.
     * This class supports the game's level system by enabling seamless transitions between different
     * levels, allowing for expansive world design. It works closely with EntityManager to populate
     * the scene with players, enemies, platforms, and interactive elements.
     */
    class LevelLoader
    {
    public:
        /**
         * @brief Loads a level from a JSON file and populates the EntityManager.
         *
         * Reads the specified JSON file, parses its contents, and creates entities in the EntityManager
         * based on the level data. This includes spawning the player, enemies, platforms, and setting
         * level-specific configuration like boundaries and physics properties. The method handles
         * error cases gracefully, returning false if the file cannot be read or parsed.
         *
         * @param entityManager The EntityManager to populate with entities from the level.
         * @param path The file system path to the JSON level file (relative to executable).
         * @return true if the level was loaded successfully and entities created, false on error.
         */
        static bool loadLevel(Gameplay::EntityManager &entityManager, const std::string &path);
    };
}
