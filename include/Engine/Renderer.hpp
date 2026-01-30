#pragma once
#include <SDL3/SDL.h>
#include <unordered_map>
#include <vector>

#include "Common/Types.hpp"

namespace Engine
{
    class Renderer
    {
    public:
        Renderer(SDL_Window *window);
        ~Renderer();

        /**
 * @brief Disable copy construction to prevent multiple instances from sharing or duplicating underlying SDL resources.
 */
Renderer(const Renderer &) = delete;
        /**
 * @brief Deleted copy assignment operator to prevent copying of Renderer instances.
 *
 * Copying is disallowed to ensure unique ownership of the underlying SDL renderer and associated resources.
 */
Renderer &operator=(const Renderer &) = delete;
        /**
 * @brief Disables move construction for Renderer.
 *
 * Prevents moving a Renderer instance to preserve unique ownership of the internal SDL resources
 * and texture cache.
 */
Renderer(Renderer &&) = delete;
        /**
 * @brief Disable move assignment for Renderer.
 *
 * Prevents transferring ownership of the Renderer and its internal SDL resources by deleting the move assignment operator.
 */
Renderer &operator=(Renderer &&) = delete;
        // TODO: add textures, bool loadTexture(int id, const std::string& path);
        void beginFrame();
        void drawCommands(const std::vector<Common::RenderCommand> &commands);
        void endFrame();

    private:
        SDL_Renderer *m_sdlRenderer;
        std::unordered_map<Common::TextureID, SDL_Texture *> m_textureCache;
    };
} // namespace Engine