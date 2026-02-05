#pragma once

#include "Gameplay/ECS/Registry.hpp"
#include "Common/Constants.hpp"

namespace Gameplay
{
    namespace Systems
    {
        class PhysicsSystem
        {
        public:
            void update(ECS::Registry& registry, float deltaTime)
            {
                // Iterate over all entities with Physics AND Transform components
                for (auto& [entity, physics] : registry.physics)
                {
                    if (!registry.has<ECS::Transform>(entity)) continue;
                    
                    auto& transform = registry.get<ECS::Transform>(entity);

                    // Apply Gravity
                    physics.velocityY += physics.gravity * deltaTime;

                    // Apply Friction if grounded and moving horizontally
                    // Apply Friction if grounded and moving horizontally
                    if (physics.isGrounded)
                    {
                        // Friction is now handled by InputSystem for Player.
                        // For non-player entities, we might want it back, but currently PhysicsSystem covers everything.
                        // Assuming generic entities don't have InputSystem driving them yet, or we accept they have no friction for now.
                        // For MVP, removing to prevent double-friction on Player.
                    }

                    // Terminal Velocity
                    if (physics.velocityY > 2000.0f) physics.velocityY = 2000.0f; // Clamp fall speed

                    // Apply Velocity to Position
                    transform.x += physics.velocityX * deltaTime;
                    transform.y += physics.velocityY * deltaTime;

                    // --- Simple World Bounds Collision (for MVP) ---
                    // Floor Collision
                    if (transform.y > Common::SCREEN_HEIGHT - transform.height)
                    {
                        transform.y = Common::SCREEN_HEIGHT - transform.height;
                        physics.velocityY = 0;
                        physics.isGrounded = true;
                    }
                    else
                    {
                         physics.isGrounded = false;
                    }
                    // Wall Collision (Left)
                    if (registry.levelConfig.isLeftWallClamped && transform.x < 0)
                    {
                        transform.x = 0;
                        physics.velocityX = 0;
                    }

                    // Wall Collision (Right)
                    if (registry.levelConfig.isRightWallClamped && transform.x > Common::WORLD_WIDTH - transform.width)
                    {
                        transform.x = Common::WORLD_WIDTH - transform.width;
                        physics.velocityX = 0;
                    }
                }
            }
        };
    }
}
