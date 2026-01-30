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

        // TODO: add textures, bool loadTexture(int id, const std::string& path);
        void beginFrame();
        void drawCommands(const std::vector<Common::RenderCommand> &commands);
        void endFrame();

    private:
        SDL_Renderer *m_sdlRenderer;
        std::unordered_map<Common::TextureID, SDL_Texture *> m_textureCache;
    };
} // namespace Engine
