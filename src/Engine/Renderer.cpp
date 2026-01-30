#include <vector>
#include <string>

#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>

#include "Engine/Renderer.hpp"

#include "Common/Types.hpp"
#include "Common/Constants.hpp"

namespace Engine
{
    /**
     * @brief Creates an SDL renderer for the specified window and configures logical presentation.
     *
     * Initializes the internal SDL_Renderer associated with the provided SDL_Window and sets
     * the renderer's logical presentation to the engine's SCREEN_WIDTH and SCREEN_HEIGHT
     * using letterbox scaling.
     *
     * @param window SDL_Window to create the renderer for; may be nullptr.
     *
     * If renderer creation fails, an error is logged and the internal renderer remains unset. */
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

    /**
     * @brief Prepares the renderer for a new frame by clearing the screen and drawing the game-area background.
     *
     * If the SDL renderer is not initialized, the call has no effect. Otherwise the render target is cleared to
     * black and the logical game area is filled with a dark gray rectangle.
     */
    void Renderer::beginFrame()
    {
        if (!m_sdlRenderer)
            return;
        SDL_SetRenderDrawColor(m_sdlRenderer, 0, 0, 0, 255);
        SDL_RenderClear(m_sdlRenderer);

        SDL_FRect gameArea = {0, 0, (float)Common::SCREEN_WIDTH, (float)Common::SCREEN_HEIGHT};
        SDL_SetRenderDrawColor(m_sdlRenderer, 30, 30, 30, 255);
        SDL_RenderFillRect(m_sdlRenderer, &gameArea);
    }

    /**
     * @brief Loads a texture from a file and caches it with the given ID.
     * @param id The texture ID to associate with the loaded texture.
     * @param path The file path to the texture image.
     * @return True if the texture was loaded successfully, false otherwise.
     */
    bool Renderer::loadTexture(Common::TextureID id, const std::string &path)
    {
        if (!m_sdlRenderer)
            return false;

        SDL_Surface *surface = IMG_Load(path.c_str());
        if (!surface)
        {
            SDL_LogError(1, "Failed to load texture from %s: %s", path.c_str(), SDL_GetError());
            return false;
        }

        SDL_Texture *texture = SDL_CreateTextureFromSurface(m_sdlRenderer, surface);
        SDL_DestroySurface(surface);

        if (!texture)
        {
            SDL_LogError(1, "Failed to create texture from surface: %s", SDL_GetError());
            return false;
        }

        m_textureCache[id] = texture;
        return true;
    }

    /**
     * @brief Releases renderer-owned GPU resources and associated cached textures.
     *
     * Destroys all SDL textures stored in the texture cache, clears the cache,
     * and destroys the underlying SDL_Renderer if one was created.
     */
    Renderer::~Renderer()
    {
        for (auto &[id, texture] : m_textureCache)
        {
            SDL_DestroyTexture(texture);
        }
        m_textureCache.clear();
        if (m_sdlRenderer)
        {
            SDL_DestroyRenderer(m_sdlRenderer);
        }
    }

    /**
     * @brief Renders a sequence of render commands to the SDL renderer.
     *
     * For each command, uses a cached SDL_Texture when available; otherwise draws a filled rectangle
     * at the command's destination. The destination rectangle is defined by the command's
     * `x`, `y`, `width`, and `height`.
     *
     * For background textures (TEX_BACKGROUND_FAR, TEX_BACKGROUND_MID, TEX_BACKGROUND_NEAR), implements tiling
     * to repeat the texture across the screen with parallax scrolling.
     *
     * When no cached texture is found, the rectangle color is chosen by texture ID:
     * - `Common::TextureID::TEX_PLAYER` → red (255,0,0,255)
     * - otherwise → cyan (0,255,255,255)
     *
     * @param commands List of render commands specifying texture IDs and destination rectangles.
     */
    void Engine::Renderer::drawCommands(const std::vector<Common::RenderCommand> &commands, float cameraOffsetX)
    {
        if (!m_sdlRenderer)
            return;
        for (const auto &cmd : commands)
        {
            SDL_FRect dest = {cmd.x - (cmd.scrollFactor * cameraOffsetX), cmd.y, cmd.width, cmd.height};
            auto it = m_textureCache.find(cmd.textureID);
            if (it != m_textureCache.end())
            {
                SDL_Texture *texture = it->second;
                // Check if it's a background texture for tiling
                if (cmd.textureID == Common::TextureID::TEX_BACKGROUND_FAR ||
                    cmd.textureID == Common::TextureID::TEX_BACKGROUND_MID ||
                    cmd.textureID == Common::TextureID::TEX_BACKGROUND_NEAR)
                {
                    // Get texture dimensions
                    float texW, texH;
                    SDL_GetTextureSize(texture, &texW, &texH);

                    // Calculate scroll offset
                    float scrollOffset = cmd.scrollFactor * cameraOffsetX;

                    // Tile horizontally and vertically
                    int startX = -(int)scrollOffset % (int)texW;
                    int startY = 0; // Assuming no vertical scrolling for simplicity, but can be extended

                    for (int y = startY; y < Common::SCREEN_HEIGHT; y += (int)texH)
                    {
                        for (int x = startX; x < Common::SCREEN_WIDTH; x += (int)texW)
                        {
                            SDL_FRect tileDest = {(float)x, (float)y, texW, texH};
                            SDL_RenderTexture(m_sdlRenderer, texture, NULL, &tileDest);
                        }
                    }
                }
                else
                {
                    SDL_RenderTexture(m_sdlRenderer, it->second, NULL, &dest);
                }
            }
            else
            {
                if (cmd.textureID == Common::TextureID::TEX_PLAYER)
                {
                    SDL_SetRenderDrawColor(m_sdlRenderer, 255, 0, 0, 255);
                }
                else if (cmd.textureID == Common::TextureID::TEX_BACKGROUND_FAR)
                {
                    SDL_SetRenderDrawColor(m_sdlRenderer, 50, 50, 100, 255); // Dark blue for far background
                }
                else if (cmd.textureID == Common::TextureID::TEX_BACKGROUND_MID)
                {
                    SDL_SetRenderDrawColor(m_sdlRenderer, 100, 100, 150, 255); // Medium blue for mid background
                }
                else if (cmd.textureID == Common::TextureID::TEX_BACKGROUND_NEAR)
                {
                    SDL_SetRenderDrawColor(m_sdlRenderer, 150, 150, 200, 255); // Light blue for near background
                }
                else
                {
                    SDL_SetRenderDrawColor(m_sdlRenderer, 0, 255, 255, 255);
                }

                SDL_RenderFillRect(m_sdlRenderer, &dest);
            }
        }
    }

    /**
     * @brief Presents the current rendered frame to the display.
     *
     * If the internal SDL_Renderer is not initialized, this call does nothing.
     */
    void Renderer::endFrame()
    {
        if (!m_sdlRenderer)
            return;
        SDL_RenderPresent(m_sdlRenderer);
    }
}