#pragma once

#include "Gameplay/ECS/Registry.hpp"
#include "Common/Types.hpp"

namespace Gameplay
{
    namespace Systems
    {
        class InputSystem
        {
        public:
            void update(ECS::Registry& registry, const Common::InputState& input, float deltaTime)
            {
                // Control any entity with a PlayerControl component
                for (auto& [entity, control] : registry.players)
                {
                    if (!registry.has<ECS::Physics>(entity)) continue;
                    
                    auto& physics = registry.get<ECS::Physics>(entity);
                    
                    // Horizontal Movement
                    if (input.left)
                    {
                        physics.velocityX = -control.speed;
                    }
                    else if (input.right)
                    {
                        physics.velocityX = control.speed;
                    }
                    else
                    {
                        // Apply Friction (Decay) instead of instant stop
                        // We use the entity's friction value
                        float drop = physics.friction * deltaTime;
                        
                        if (physics.velocityX > 0)
                        {
                            physics.velocityX -= drop;
                            if (physics.velocityX < 0) physics.velocityX = 0;
                        }
                        else if (physics.velocityX < 0)
                        {
                            physics.velocityX += drop;
                            if (physics.velocityX > 0) physics.velocityX = 0;
                        }
                    }
                    
                    // Jumping
                    if (input.jump && physics.isGrounded && control.canJump)
                    {
                        physics.velocityY = Common::JUMP_FORCE;
                        physics.isGrounded = false;
                        control.canJump = false; // Debounce or logic to prevent multijump
                    }
                    
                    // Reset jump flag if not holding space (simple logic)
                    if (!input.jump)
                    {
                        control.canJump = true;
                    }
                }
            }
        };
    }
}
