/**
 * @file LevelLoader.cpp
 * @brief Implementation of the LevelLoader class for parsing JSON level data.
 *
 * This file provides the concrete implementation of level loading functionality,
 * converting JSON level definitions into runtime entity objects. It supports
 * various entity types (players, static objects, background layers) with their
 * associated components (transform, sprite, physics, colliders, etc.). The loader
 * includes a flexible dimension resolution system that allows levels to use
 * symbolic constants for positioning and sizing.
 */

#include "Engine/LevelLoader.hpp"

#include <fstream>
#include <iostream>
#include <SDL3/SDL.h>

#include "nlohmann/json.hpp" // nlohmann/json
#include "Common/Constants.hpp"

using json = nlohmann::json;

namespace Engine
{
    namespace
    {
        // Helper function to resolve dimension values from JSON, supporting both numeric and symbolic values
        float resolveDimension(const json &j, const std::string &key, float defaultValue, float entitySize = 0.0f)
        {
            if (!j.contains(key))
                return defaultValue;

            const auto &val = j[key];
            if (val.is_number())
            {
                return val.get<float>();
            }
            else if (val.is_string())
            {
                std::string s = val.get<std::string>();
                if (s == "SCREEN_WIDTH")
                    return static_cast<float>(Common::SCREEN_WIDTH);
                if (s == "SCREEN_HEIGHT")
                    return static_cast<float>(Common::SCREEN_HEIGHT);
                if (s == "WORLD_WIDTH")
                    return static_cast<float>(Common::WORLD_WIDTH);
                if (s == "WORLD_START")
                    return static_cast<float>(Common::WORLD_START);
                if (s == "WORLD_END")
                    return static_cast<float>(Common::WORLD_WIDTH);
                if (s == "CENTER_X")
                    return (static_cast<float>(Common::SCREEN_WIDTH) - entitySize) / 2.0f;
                if (s == "CENTER_Y")
                    return (static_cast<float>(Common::SCREEN_HEIGHT) - entitySize) / 2.0f;
                if (s == "FLOOR")
                    return static_cast<float>(Common::SCREEN_HEIGHT) - entitySize;
            }
            return defaultValue;
        }
    }

    bool LevelLoader::loadLevel(Gameplay::EntityManager &entityManager, const std::string &path)
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
        catch (const std::exception &e)
        {
            SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Error reading level file %s: %s", path.c_str(), e.what());
            return false;
        }

        // Parse Level Configuration
        entityManager.levelConfig.isLeftWallClamped = levelData.value("isLeftWallClamped", true);
        entityManager.levelConfig.isRightWallClamped = levelData.value("isRightWallClamped", true);
        entityManager.levelConfig.levelWidth = levelData.value("levelWidth", 2560.0f);

        // Iterate over entities array
        if (levelData.contains("entities") && levelData["entities"].is_array())
        {
            for (const auto &entityDef : levelData["entities"])
            {
                std::shared_ptr<Gameplay::Entity> entity;

                // Determine entity type
                if (entityDef.contains("playerControl"))
                {
                    entity = std::make_shared<Gameplay::Player>();
                }
                else if (entityDef.contains("parallax"))
                {
                    entity = std::make_shared<Gameplay::BackgroundLayer>();
                }
                else
                {
                    entity = std::make_shared<Gameplay::StaticObject>();
                }

                // 1. Transform Component
                if (entityDef.contains("transform"))
                {
                    const auto &t = entityDef["transform"];

                    // Parse size first to use in position calculation
                    entity->transform.width = resolveDimension(t, "width", 32.0f);
                    entity->transform.height = resolveDimension(t, "height", 32.0f);

                    // Parse position using size (for centering or aligning)
                    entity->transform.x = resolveDimension(t, "x", 0.0f, entity->transform.width);
                    entity->transform.y = resolveDimension(t, "y", 0.0f, entity->transform.height);
                    entity->transform.zIndex = t.value("zIndex", 0);
                }

                // 2. Sprite Component
                if (entityDef.contains("sprite"))
                {
                    const auto &s = entityDef["sprite"];
                    Gameplay::Sprite sprite;
                    sprite.textureID = static_cast<Common::TextureID>(s.value("textureID", -1));
                    sprite.zIndex = s.value("zIndex", 0);
                    entity->sprite = sprite;

                    // If transform zIndex was not explicitly set, use sprite zIndex
                    if (!entityDef.contains("transform") || !entityDef["transform"].contains("zIndex"))
                    {
                        entity->transform.zIndex = sprite.zIndex;
                    }
                }

                // 3. Physics Component
                if (entityDef.contains("physics"))
                {
                    const auto &p = entityDef["physics"];
                    Gameplay::Physics physics;
                    physics.velocityX = p.value("velocityX", 0.0f);
                    physics.velocityY = p.value("velocityY", 0.0f);
                    entity->physics = physics;
                }

                // 4. Parallax Component
                if (entityDef.contains("parallax"))
                {
                    const auto &p = entityDef["parallax"];
                    Gameplay::Parallax parallax;
                    parallax.factor = p.value("factor", 1.0f);
                    entity->parallax = parallax;
                }

                // 5. PlayerControl Tag
                if (entityDef.contains("playerControl"))
                {
                    const auto &pc = entityDef["playerControl"];
                    Gameplay::PlayerControl playerControl;
                    playerControl.speed = pc.value("speed", 200.0f);
                    entity->playerControl = playerControl;
                }

                // 6. Collider Component
                if (entityDef.contains("collider"))
                {
                    const auto &c = entityDef["collider"];
                    Gameplay::Collider collider;
                    collider.width = c.value("width", 32.0f);
                    collider.height = c.value("height", 32.0f);
                    collider.offsetX = c.value("offsetX", 0.0f);
                    collider.offsetY = c.value("offsetY", 0.0f);
                    collider.isTrigger = c.value("isTrigger", false);
                    collider.isSolid = c.value("isSolid", false);
                    entity->collider = collider;
                }

                // 7. LevelExit Component
                if (entityDef.contains("exit"))
                {
                    const auto &e = entityDef["exit"];
                    Gameplay::LevelExit exit;

                    std::string dirStr = e.value("direction", "East");
                    if (dirStr == "North")
                        exit.direction = Gameplay::CardinalDirection::North;
                    else if (dirStr == "South")
                        exit.direction = Gameplay::CardinalDirection::South;
                    else if (dirStr == "West")
                        exit.direction = Gameplay::CardinalDirection::West;
                    else
                        exit.direction = Gameplay::CardinalDirection::East;

                    exit.nextLevelPath = e.value("nextLevel", "");
                    exit.transitionDuration = e.value("transitionDuration", 0.5f);
                    entity->exit = exit;
                }

                entityManager.addEntity(entity);
            }
        }

        SDL_Log("Level loaded successfully: %s", path.c_str());
        return true;
    }
}
