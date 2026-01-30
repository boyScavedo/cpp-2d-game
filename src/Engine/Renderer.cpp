#include <vector>

#include "Engine/Renderer.hpp"

#include "Common/Types.hpp"
#include "Common/Constants.hpp"

namespace Engine
{
    Renderer::Renderer(SDL_Window *window)
    {
        m_sdlRenderer = SDL_CreateRenderer(window, NULL);

        if (!m_sdlRenderer)
        {
            SDL_LogError(1, "Failed to create renderer");
            return;
        }

        SDL_SetRenderLogicalPresentation(m_sdlRenderer, Common::SCREEN_WIDTH, Common::SCREEN_HEIGHT, SDL_LOGICAL_PRESENTATION_LETTERBOX);
    }

    void Renderer::beginFrame()
    {
        SDL_SetRenderDrawColor(m_sdlRenderer, 0, 0, 0, 255);
        SDL_RenderClear(m_sdlRenderer);
    }

    Renderer::~Renderer()
    {
        for (auto &[id, texture] : m_textureCache)
        {
            SDL_DestroyTexture(texture);
        }
        m_textureCache.clear();
        SDL_DestroyRenderer(m_sdlRenderer);
    }

    void Renderer::drawCommands(const std::vector<Common::RenderCommand> &commands)
    {
        for (const auto &cmd : commands)
        {
            SDL_FRect dest = {cmd.x, cmd.y, cmd.width, cmd.height};
            auto it = m_textureCache.find(cmd.textureID);
            if (it != m_textureCache.end())
            {
                SDL_RenderTexture(m_sdlRenderer, it->second, NULL, &dest);
            }
            else
            {
                if (cmd.textureID == Common::TextureID::TEX_PLAYER)
                {
                    SDL_SetRenderDrawColor(m_sdlRenderer, 255, 0, 0, 255);
                }
                else
                {
                    SDL_SetRenderDrawColor(m_sdlRenderer, 0, 255, 255, 255);
                }

                SDL_RenderFillRect(m_sdlRenderer, &dest);
            }
        }
    }

    void Renderer::endFrame()
    {
        SDL_RenderPresent(m_sdlRenderer);
    }
}