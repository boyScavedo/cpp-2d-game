#include "Engine/LevelLoader.hpp"

#include <fstream>
#include <iostream>
#include <SDL3/SDL.h>

#include "Utils/json.hpp" // nlohmann/json
#include "Common/Constants.hpp"

using json = nlohmann::json;

namespace Engine
{
    namespace
    {
        float resolveDimension(const json& j, const std::string& key, float defaultValue, float entitySize = 0.0f)
        {
            if (!j.contains(key)) return defaultValue;

            const auto& val = j[key];
            if (val.is_number())
            {
                return val.get<float>();
            }
            else if (val.is_string())
            {
                std::string s = val.get<std::string>();
                if (s == "SCREEN_WIDTH") return static_cast<float>(Common::SCREEN_WIDTH);
                if (s == "SCREEN_HEIGHT") return static_cast<float>(Common::SCREEN_HEIGHT);
                if (s == "WORLD_WIDTH") return static_cast<float>(Common::WORLD_WIDTH);
                if (s == "WORLD_START") return static_cast<float>(Common::WORLD_START);
                if (s == "WORLD_END") return static_cast<float>(Common::WORLD_WIDTH);
                if (s == "CENTER_X") return (static_cast<float>(Common::SCREEN_WIDTH) - entitySize) / 2.0f;
                if (s == "CENTER_Y") return (static_cast<float>(Common::SCREEN_HEIGHT) - entitySize) / 2.0f;
                if (s == "FLOOR") return static_cast<float>(Common::SCREEN_HEIGHT) - entitySize;
            }
            return defaultValue;
        }
    }

    bool LevelLoader::loadLevel(Gameplay::ECS::Registry &registry, const std::string &path)
    {
        std::ifstream file(path);
        if (!file.is_open())
        {
            SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to open level file: %s", path.c_str());
            return false;
        }

        json levelData;
        try
        {
            file >> levelData;
        }
        catch (const json::parse_error &e)
        {
            SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "JSON Parse Error in %s: %s", path.c_str(), e.what());
            return false;
        }

        // Parse Level Configuration
        registry.levelConfig.isLeftWallClamped = levelData.value("isLeftWallClamped", true);
        registry.levelConfig.isRightWallClamped = levelData.value("isRightWallClamped", true);

        // Iterate over entities array
        if (levelData.contains("entities") && levelData["entities"].is_array())
        {
            for (const auto &entityDef : levelData["entities"])
            {
                auto entity = registry.createEntity();

                // 1. Transform Component
                if (entityDef.contains("transform"))
                {
                    const auto &t = entityDef["transform"];
                    Gameplay::ECS::Transform transform;
                    
                    // Parse size first to use in position calculation
                    transform.width = resolveDimension(t, "width", 32.0f);
                    transform.height = resolveDimension(t, "height", 32.0f);
                    
                    // Parse position using size (for centering or aligning)
                    transform.x = resolveDimension(t, "x", 0.0f, transform.width);
                    transform.y = resolveDimension(t, "y", 0.0f, transform.height);
                    
                    registry.add(entity, transform);
                }

                // 2. Sprite Component
                if (entityDef.contains("sprite"))
                {
                    const auto &s = entityDef["sprite"];
                    Gameplay::ECS::Sprite sprite;
                    sprite.textureID = static_cast<Common::TextureID>(s.value("textureID", -1));
                    sprite.zIndex = s.value("zIndex", 0);
                    registry.add(entity, sprite);
                }

                // 3. Physics Component
                if (entityDef.contains("physics"))
                {
                    const auto &p = entityDef["physics"];
                    Gameplay::ECS::Physics physics;
                    // Usually velocity starts at 0, but can be overridden
                    physics.velocityX = p.value("velocityX", 0.0f);
                    physics.velocityY = p.value("velocityY", 0.0f);
                    registry.add(entity, physics);
                }
                
                 // 4. Parallax Component
                if (entityDef.contains("parallax"))
                {
                    const auto &p = entityDef["parallax"];
                    Gameplay::ECS::Parallax parallax;
                    parallax.factor = p.value("factor", 1.0f);
                    registry.add(entity, parallax);
                }

                // 5. PlayerControl Tag
                if (entityDef.contains("playerControl"))
                {
                    const auto &pc = entityDef["playerControl"];
                    Gameplay::ECS::PlayerControl playerControl;
                    playerControl.speed = pc.value("speed", 200.0f);
                    registry.add(entity, playerControl);
                }

                // 6. Collider Component
                if (entityDef.contains("collider"))
                {
                    const auto &c = entityDef["collider"];
                    Gameplay::ECS::Collider collider;
                    collider.width = c.value("width", 32.0f);
                    collider.height = c.value("height", 32.0f);
                    collider.offsetX = c.value("offsetX", 0.0f);
                    collider.offsetY = c.value("offsetY", 0.0f);
                    collider.isTrigger = c.value("isTrigger", false);
                    registry.add(entity, collider);
                }

                // 7. LevelExit Component
                if (entityDef.contains("exit"))
                {
                    const auto &e = entityDef["exit"];
                    Gameplay::ECS::LevelExit exit;
                    
                    std::string dirStr = e.value("direction", "East");
                    if (dirStr == "North") exit.direction = Gameplay::ECS::CardinalDirection::North;
                    else if (dirStr == "South") exit.direction = Gameplay::ECS::CardinalDirection::South;
                    else if (dirStr == "West") exit.direction = Gameplay::ECS::CardinalDirection::West;
                    else exit.direction = Gameplay::ECS::CardinalDirection::East;

                    exit.nextLevelPath = e.value("nextLevel", "");
                    exit.transitionDuration = e.value("transitionDuration", 0.5f);
                    registry.add(entity, exit);
                }
            }
        }
        
        SDL_Log("Level loaded successfully: %s", path.c_str());
        return true;
    }
}
