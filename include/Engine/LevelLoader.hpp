#pragma once

#include <string>
#include "Gameplay/ECS/Registry.hpp"

namespace Engine
{
    class LevelLoader
    {
    public:
        /**
         * @brief Loads a level from a JSON file and populates the ECS Registry.
         * 
         * @param registry The ECS registry to populate with entities.
         * @param path The path to the JSON level file.
         * @return true if successful, false otherwise.
         */
        static bool loadLevel(Gameplay::ECS::Registry& registry, const std::string& path);
    };
}
