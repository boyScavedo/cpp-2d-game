#include "Common/Types.hpp"
#include "Gameplay/EntityManager.hpp"
#include "nlohmann/json.hpp"
#include <algorithm>

using json = nlohmann::json;

namespace Gameplay
{
    void EntityManager::update(float deltaTime, const Common::InputState& input)
    {
        for (auto& entity : m_entities) {
            if (entity->isDestroyed) continue;

            // Handle player input separately
            if (auto player = std::dynamic_pointer_cast<Player>(entity)) {
                player->handleInput(input);
            }

            // Update entity
            entity->update(deltaTime);

            // World Bound Constraints (Floor)
            if (entity->physics) {
                if (entity->transform.y > Common::SCREEN_HEIGHT - entity->transform.height) {
                    entity->transform.y = Common::SCREEN_HEIGHT - entity->transform.height;
                    entity->physics->velocityY = 0;
                    entity->physics->isGrounded = true;
                } else {
                    entity->physics->isGrounded = false;
                }

                // Wall Collision (Left)
                if (levelConfig.isLeftWallClamped && entity->transform.x < 0) {
                    entity->transform.x = 0;
                    entity->physics->velocityX = 0;
                }

                // Wall Collision (Right)
                if (levelConfig.isRightWallClamped && entity->transform.x > levelConfig.levelWidth - entity->transform.width) {
                    entity->transform.x = levelConfig.levelWidth - entity->transform.width;
                    entity->physics->velocityX = 0;
                }
            }
        }

        // Remove destroyed entities
        m_entities.erase(
            std::remove_if(m_entities.begin(), m_entities.end(),
                [](const std::shared_ptr<Entity>& e) { return e->isDestroyed; }),
            m_entities.end()
        );
    }

    void EntityManager::render(std::vector<Common::RenderCommand>& commands)
    {
        // Sort entities by zIndex before rendering
        auto sortedEntities = m_entities;
        std::sort(sortedEntities.begin(), sortedEntities.end(), [](const std::shared_ptr<Entity>& a, const std::shared_ptr<Entity>& b) {
            return a->transform.zIndex < b->transform.zIndex;
        });

        for (auto& entity : sortedEntities) {
            entity->render(commands);
        }
    }

    void EntityManager::addEntity(std::shared_ptr<Entity> entity)
    {
        if (entity->id == -1) {
            entity->id = m_nextEntityID++;
        }
        m_entities.push_back(entity);
    }

    void EntityManager::clear()
    {
        m_entities.clear();
    }

    std::shared_ptr<Player> EntityManager::getPlayer() const
    {
        for (const auto& entity : m_entities) {
            if (auto player = std::dynamic_pointer_cast<Player>(entity)) {
                return player;
            }
        }
        return nullptr;
    }

    std::optional<LevelExit> EntityManager::checkCollisions()
    {
        auto player = getPlayer();
        if (!player || !player->collider) return std::nullopt;

        std::optional<LevelExit> exitTriggered = std::nullopt;

        float pLeft = player->transform.x + player->collider->offsetX;
        float pRight = pLeft + player->collider->width;
        float pTop = player->transform.y + player->collider->offsetY;
        float pBottom = pTop + player->collider->height;

        for (const auto& entity : m_entities) {
            if (entity == player) continue;
            if (!entity->collider) continue;

            float oLeft = entity->transform.x + entity->collider->offsetX;
            float oRight = oLeft + entity->collider->width;
            float oTop = entity->transform.y + entity->collider->offsetY;
            float oBottom = oTop + entity->collider->height;

            // AABB Overlap Check
            bool collision = (pLeft < oRight && pRight > oLeft &&
                              pTop < oBottom && pBottom > oTop);

            if (collision) {
                // Handle Level Exits
                if (entity->exit) {
                    exitTriggered = entity->exit;
                }

                // Handle Solid Collisions (Resolution)
                if (entity->collider->isSolid) {
                    float overlapX = std::min(pRight - oLeft, oRight - pLeft);
                    float overlapY = std::min(pBottom - oTop, oBottom - pTop);

                    if (overlapX < overlapY) {
                        // Resolve on X axis
                        if (pLeft < oLeft) player->transform.x -= overlapX;
                        else player->transform.x += overlapX;
                        if (player->physics) player->physics->velocityX = 0;
                    } else {
                        // Resolve on Y axis
                        if (pTop < oTop) {
                            player->transform.y -= overlapY;
                            if (player->physics) {
                                player->physics->velocityY = 0;
                                player->physics->isGrounded = true;
                            }
                        } else {
                            player->transform.y += overlapY;
                            if (player->physics) player->physics->velocityY = 0;
                        }
                    }
                    // Re-calculate p bounds for next entity check
                    pLeft = player->transform.x + player->collider->offsetX;
                    pRight = pLeft + player->collider->width;
                    pTop = player->transform.y + player->collider->offsetY;
                    pBottom = pTop + player->collider->height;
                }
            }
        }

        return exitTriggered;
    }

    void EntityManager::removeEntity(std::shared_ptr<Entity> entity)
    {
        m_entities.erase(
            std::remove(m_entities.begin(), m_entities.end(), entity),
            m_entities.end()
        );
    }

    void EntityManager::removeEntity(int entityID)
    {
        m_entities.erase(
            std::remove_if(m_entities.begin(), m_entities.end(),
                           [entityID](const std::shared_ptr<Entity> &e)
                           { return e->id == entityID; }),
            m_entities.end());
    }

    std::string EntityManager::toJSON() const
    {
        nlohmann::json levelData;
        levelData["isLeftWallClamped"] = levelConfig.isLeftWallClamped;
        levelData["isRightWallClamped"] = levelConfig.isRightWallClamped;
        levelData["levelWidth"] = levelConfig.levelWidth;

        levelData["entities"] = nlohmann::json::array();
        for (const auto& entity : m_entities) {
            nlohmann::json e;
            
            // Type identification
            if (std::dynamic_pointer_cast<Player>(entity)) e["type"] = "Player";
            else if (std::dynamic_pointer_cast<BackgroundLayer>(entity)) e["type"] = "Background";
            else e["type"] = "StaticObject";

            e["id"] = entity->id;

            // Transform
            e["transform"] = {
                {"x", entity->transform.x},
                {"y", entity->transform.y},
                {"width", entity->transform.width},
                {"height", entity->transform.height},
                {"zIndex", entity->transform.zIndex}
            };

            // Sprite
            if (entity->sprite) {
                e["sprite"] = {
                    {"textureID", static_cast<int>(entity->sprite->textureID)},
                    {"zIndex", entity->sprite->zIndex}
                };
            }

            // Collider
            if (entity->collider) {
                e["collider"] = {
                    {"width", entity->collider->width},
                    {"height", entity->collider->height},
                    {"offsetX", entity->collider->offsetX},
                    {"offsetY", entity->collider->offsetY},
                    {"isSolid", entity->collider->isSolid},
                    {"isTrigger", entity->collider->isTrigger}
                };
            }

            // Component data
            if (entity->playerControl) {
                e["playerControl"] = {
                    {"speed", entity->playerControl->speed}
                };
            }
            if (entity->parallax) {
                e["parallax"] = {
                    {"factor", entity->parallax->factor}
                };
            }
            if (entity->exit) {
                std::string dir = "East";
                if (entity->exit->direction == CardinalDirection::West) dir = "West";
                else if (entity->exit->direction == CardinalDirection::North) dir = "North";
                else if (entity->exit->direction == CardinalDirection::South) dir = "South";

                e["exit"] = {
                    {"direction", dir},
                    {"nextLevel", entity->exit->nextLevelPath},
                    {"transitionDuration", entity->exit->transitionDuration}
                };
            }

            levelData["entities"].push_back(e);
        }

        return levelData.dump(4);
    }
}
