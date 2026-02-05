#pragma once

#include <vector>
#include "Gameplay/ECS/Registry.hpp"
#include "Common/Types.hpp"
#include "Engine/Renderer.hpp" // For Texture definitions if needed

namespace Gameplay
{
    namespace Systems
    {
        class RenderSystem
        {
        public:
            /**
             * @brief Collects render commands from the ECS registry.
             * 
             * @param registry The ECS registry.
             * @param outCommands The vector to populate with RenderCommands.
             */
            void render(const ECS::Registry& registry, std::vector<Common::RenderCommand>& outCommands)
            {
                // 1. Render Backgrounds (Parallax)
                for (const auto& [entity, parallax] : registry.backgrounds)
                {
                    if (registry.has<ECS::Sprite>(entity) && registry.has<ECS::Transform>(entity))
                    {
                         const auto& sprite = registry.get<ECS::Sprite>(entity);
                         const auto& transform = registry.get<ECS::Transform>(entity);
                         
                         Common::RenderCommand cmd;
                         cmd.x = transform.x;
                         cmd.y = transform.y;
                         cmd.width = transform.width;
                         cmd.height = transform.height;
                         cmd.textureID = sprite.textureID;
                         cmd.scrollFactor = parallax.factor;
                         
                         outCommands.push_back(cmd);
                    }
                }
                
                // 2. Render Sprites (Entities like Player)
                // Note: ideally we sort by zIndex. For MVP we just iterate.
                for (const auto& [entity, sprite] : registry.sprites)
                {
                    // Skip backgrounds (already rendered)
                    if (registry.has<ECS::Parallax>(entity)) continue;

                    if (registry.has<ECS::Transform>(entity))
                    {
                        const auto& transform = registry.get<ECS::Transform>(entity);
                        
                        Common::RenderCommand cmd;
                        cmd.x = transform.x;
                        cmd.y = transform.y;
                        cmd.width = transform.width;
                        cmd.height = transform.height;
                        cmd.textureID = sprite.textureID;
                        cmd.scrollFactor = 1.0f; // Default scroll factor for normal entities
                        
                        outCommands.push_back(cmd);
                    }
                }
            }
        };
    }
}
