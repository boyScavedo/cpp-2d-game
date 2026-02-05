#pragma once

#include "Gameplay/ECS/Registry.hpp"
#include <optional>
#include <iostream>

namespace Gameplay
{
    namespace Systems
    {
        class CollisionSystem
        {
        public:
            /**
             * @brief Checks for collisions and returns the LevelExit data if triggered.
             * 
             * @param registry The ECS registry.
             * @return std::optional<ECS::LevelExit> Exit data if triggered, std::nullopt otherwise.
             */
            std::optional<ECS::LevelExit> check(ECS::Registry& registry)
            {
                // Get all entities with Colliders
                // Note: Ideally we use a spatial partition (QuadTree) for performance.

                // We are primarily interested in PLAYER vs OTHERS interactions for now.
                for (auto& [playerEntity, playerControl] : registry.players)
                {
                    if (!registry.has<ECS::Collider>(playerEntity) || !registry.has<ECS::Transform>(playerEntity)) continue;

                    auto& playerTransform = registry.get<ECS::Transform>(playerEntity);
                    auto& playerCollider = registry.get<ECS::Collider>(playerEntity);

                    // Calculate Player AABB
                    float pLeft = playerTransform.x + playerCollider.offsetX;
                    float pRight = pLeft + playerCollider.width;
                    float pTop = playerTransform.y + playerCollider.offsetY;
                    float pBottom = pTop + playerCollider.height;

                    // Check against all other colliders
                    for (auto& [otherEntity, otherCollider] : registry.colliders)
                    {
                        if (playerEntity == otherEntity) continue; // Don't check self
                        if (!registry.has<ECS::Transform>(otherEntity)) continue;

                        auto& otherTransform = registry.get<ECS::Transform>(otherEntity);

                        // Calculate Other AABB
                        float oLeft = otherTransform.x + otherCollider.offsetX;
                        float oRight = oLeft + otherCollider.width;
                        float oTop = otherTransform.y + otherCollider.offsetY;
                        float oBottom = oTop + otherCollider.height;

                        // AABB Intersection Test (Inclusive)
                        bool collision = (pLeft <= oRight && pRight >= oLeft &&
                                          pTop <= oBottom && pBottom >= oTop);

                        if (collision)
                        {
                            // Handle Level Exit Trigger
                            if (registry.has<ECS::LevelExit>(otherEntity))
                            {
                                return registry.get<ECS::LevelExit>(otherEntity);
                            }

                            // Handle Physics Blocking (To be implemented later)
                            // if (!otherCollider.isTrigger) { ... resolve overlap ... }
                        }
                    }
                }

                return std::nullopt;
            }
        };
    }
}
