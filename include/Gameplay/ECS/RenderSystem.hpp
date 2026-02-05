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
                // We collect all commands first to sort them by zIndex for deterministic layering
                struct SortableCommand {
                    int zIndex;
                    Common::RenderCommand command;
                };
                std::vector<SortableCommand> sortableCommands;

                // 1. Render Backgrounds (Parallax)
                for (const auto& [entity, parallax] : registry.backgrounds)
                {
                    if (registry.has<ECS::Transform>(entity))
                    {
                         const auto& transform = registry.get<ECS::Transform>(entity);
                         
                         Common::RenderCommand cmd;
                         cmd.x = transform.x;
                         cmd.y = transform.y;
                         cmd.width = transform.width;
                         cmd.height = transform.height;
                         cmd.textureID = Common::TextureID::TEX_NONE;
                         cmd.scrollFactor = parallax.factor;

                         int zIndex = transform.zIndex; // Fallback to transform's zIndex
                         
                         if (registry.has<ECS::Sprite>(entity))
                         {
                             const auto& sprite = registry.get<ECS::Sprite>(entity);
                             cmd.textureID = sprite.textureID;
                             // Use sprite zIndex if it's specifically set (e.g. override)
                             // or just keep transform's if sprite's is 0 and transform's is not
                             if (sprite.zIndex != 0) zIndex = sprite.zIndex;
                         }
                         
                         sortableCommands.push_back({ zIndex, cmd });
                    }
                }
                
                // 2. Render Sprites (Entities like Player)
                for (const auto& [entity, sprite] : registry.sprites)
                {
                    // Skip backgrounds (already handled above)
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
                        cmd.scrollFactor = 1.0f; 

                        int zIndex = sprite.zIndex != 0 ? sprite.zIndex : transform.zIndex;
                        sortableCommands.push_back({ zIndex, cmd });
                    }
                }

                // 3. Sort by zIndex (Ascending)
                std::sort(sortableCommands.begin(), sortableCommands.end(), [](const SortableCommand& a, const SortableCommand& b) {
                    return a.zIndex < b.zIndex;
                });

                // 4. Final collection
                for (auto& sc : sortableCommands)
                {
                    outCommands.push_back(sc.command);
                }
            }
        };
    }
}
