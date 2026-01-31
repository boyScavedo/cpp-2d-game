#pragma once
#include <SDL3/SDL.h>
#include <unordered_map>
#include <vector>

#include "Common/Types.hpp"

namespace Engine
{
    /**
     * @class Renderer
     * @brief Handles rendering operations using SDL, managing textures and drawing commands.
     *
     * The Renderer class creates and manages an SDL_Renderer, caches textures, and provides methods
     * to draw render commands with support for parallax scrolling.
     */
    class Renderer
    {
    public:
        /**
         * @brief Constructs a Renderer with an SDL window.
         * @param window The SDL window to associate with the renderer.
         */
        Renderer(SDL_Window *window);

        /**
         * @brief Destroys the Renderer and cleans up SDL resources.
         */
        ~Renderer();

        /**
         * @brief Deleted copy constructor to prevent multiple instances from sharing SDL resources.
         */
        Renderer(const Renderer &) = delete;

        /**
         * @brief Deleted copy assignment operator to ensure unique ownership of SDL resources.
         */
        Renderer &operator=(const Renderer &) = delete;

        /**
         * @brief Deleted move constructor to preserve unique ownership of SDL resources.
         */
        Renderer(Renderer &&) = delete;

        /**
         * @brief Deleted move assignment operator to prevent transferring SDL resource ownership.
         */
        Renderer &operator=(Renderer &&) = delete;
        /**
         * @brief Loads a texture from a file and caches it with the given ID.
         * @param id The texture ID to associate with the loaded texture.
         * @param path The file path to the texture image.
         * @return True if the texture was loaded successfully, false otherwise.
         */
        bool loadTexture(Common::TextureID id, const std::string &path);

        /**
         * @brief Prepares the renderer for drawing a new frame.
         */
        void beginFrame();

        /**
         * @brief Draws a list of render commands with an optional camera offset for parallax effects.
         * @param commands The vector of render commands to draw.
         * @param cameraOffsetX The horizontal camera offset to apply.
         */
        void drawCommands(const std::vector<Common::RenderCommand> &commands, float cameraOffsetX = 0.0f);

        /**
         * @brief Finalizes and presents the current frame to the screen.
         */
        void endFrame();

    private:
        SDL_Renderer *m_sdlRenderer;
        std::unordered_map<Common::TextureID, SDL_Texture *> m_textureCache;
    };
} // namespace Engine