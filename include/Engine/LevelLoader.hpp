#pragma once

#include <string>
#include "Gameplay/EntityManager.hpp"

namespace Engine
{
    class LevelLoader
    {
    public:
        /**
         * @brief Loads a level from a JSON file and populates the EntityManager.
         * 
         * @param entityManager The EntityManager to populate with entities.
         * @param path The path to the JSON level file.
         * @return true if successful, false otherwise.
         */
        static bool loadLevel(Gameplay::EntityManager& entityManager, const std::string& path);
    };
}
