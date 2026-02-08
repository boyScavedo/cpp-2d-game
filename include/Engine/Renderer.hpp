#pragma once
#include <SDL3/SDL.h>
#include <unordered_map>
#include <vector>

#include "Common/Types.hpp"

namespace Engine
{
    /**
     * @class Renderer
     * @brief Handles all rendering operations using SDL, managing textures and drawing commands.
     *
     * The Renderer is the core graphics component of the engine, responsible for translating
     * high-level render commands into actual pixel output. It manages an SDL_Renderer for hardware-accelerated
     * drawing, caches textures for performance, and handles camera transformations for parallax scrolling.
     * This class bridges the game's logical rendering requests (from Gameplay systems) with the display
     * hardware, supporting both world-space rendering (with camera offsets) and screen-space UI rendering.
     * It ensures efficient batching of draw calls and proper resource management to maintain smooth framerates.
     */
    class Renderer
    {
    public:
        /**
         * @brief Constructs a Renderer with an SDL window.
         *
         * Creates an SDL_Renderer associated with the provided window, setting up the graphics context
         * for the entire game. This establishes the rendering pipeline that will handle all visual output.
         *
         * @param window The SDL window to associate with the renderer (must remain valid during renderer's lifetime).
         */
        Renderer(SDL_Window *window);

        /**
         * @brief Destroys the Renderer and cleans up SDL resources.
         *
         * Releases the SDL_Renderer and all cached textures, ensuring no memory leaks.
         * Called automatically when the Renderer goes out of scope.
         */
        ~Renderer();

        /**
         * @brief Deleted copy constructor to prevent multiple instances from sharing SDL resources.
         *
         * SDL resources cannot be safely shared; each Renderer must have exclusive ownership.
         */
        Renderer(const Renderer &) = delete;

        /**
         * @brief Deleted copy assignment operator to ensure unique ownership of SDL resources.
         *
         * Prevents accidental transfer of SDL resource ownership between Renderer instances.
         */
        Renderer &operator=(const Renderer &) = delete;

        /**
         * @brief Deleted move constructor to preserve unique ownership of SDL resources.
         *
         * Moving renderers would complicate resource management; prefer explicit ownership transfer.
         */
        Renderer(Renderer &&) = delete;

        /**
         * @brief Deleted move assignment operator to prevent transferring SDL resource ownership.
         *
         * Maintains single ownership of SDL resources for safety and predictability.
         */
        Renderer &operator=(Renderer &&) = delete;

        /**
         * @brief Loads a texture from a file and caches it with the given ID.
         *
         * Loads an image file into GPU memory as an SDL_Texture, associating it with a TextureID
         * for efficient reuse. This method supports the game's asset management by caching textures
         * to avoid repeated disk I/O during gameplay.
         *
         * @param id The texture ID to associate with the loaded texture (used for drawing commands).
         * @param path The file system path to the texture image (PNG, JPG, etc.).
         * @return True if the texture was loaded and cached successfully, false on error (file not found, invalid format).
         */
        bool loadTexture(Common::TextureID id, const std::string &path);

        /**
         * @brief Prepares the renderer for drawing a new frame by clearing the screen.
         *
         * Clears the back buffer with the default background color, setting up a clean slate
         * for the new frame's rendering operations. This is the first step in the frame rendering pipeline.
         */
        void beginFrame();

        /**
         * @brief Draws a list of render commands with an optional camera offset for parallax effects.
         *
         * Processes a batch of RenderCommand objects, applying camera transformations for world-space rendering.
         * Supports parallax scrolling by adjusting positions based on cameraOffsetX, enabling layered backgrounds.
         * This method handles the bulk of gameplay rendering, including entities, tiles, and effects.
         *
         * @param commands The vector of render commands to draw (each containing position, size, texture, etc.).
         * @param cameraOffsetX The horizontal camera offset to apply for parallax and world positioning.
         */
        void drawCommands(const std::vector<Common::RenderCommand> &commands, float cameraOffsetX = 0.0f);

        /**
         * @brief Draws a list of render commands in fixed UI space (no camera offset).
         *
         * Renders UI elements that should remain stationary on screen, ignoring camera movement.
         * Used for menus, HUD elements, and editor interfaces that don't scroll with the world.
         *
         * @param commands The vector of render commands to draw in screen space.
         */
        void drawUI(const std::vector<Common::RenderCommand> &commands);

        /**
         * @brief Draws a full-screen black overlay with the specified alpha transparency.
         *
         * Applies a black overlay across the entire screen for effects like fade-to-black transitions,
         * pause menus, or level loading screens. The alpha value controls transparency level.
         *
         * @param alpha The alpha value (0.0f = fully transparent, 255.0f = fully opaque).
         */
        void drawOverlay(float alpha);

        /**
         * @brief Finalizes and presents the current frame to the screen.
         *
         * Swaps the back buffer with the front buffer, making the rendered frame visible.
         * This completes the frame rendering pipeline and should be called last in each frame.
         */
        void endFrame();

        /**
         * @brief Returns the raw SDL_Renderer pointer for coordinate conversion.
         *
         * Provides access to the underlying SDL_Renderer for advanced operations like
         * mouse coordinate conversion (used by InputManager). Ownership remains with the Renderer.
         *
         * @return SDL_Renderer* The internal SDL renderer (do not destroy or modify externally).
         */
        SDL_Renderer *getSDLRenderer() const { return m_sdlRenderer; }

    private:
        SDL_Renderer *m_sdlRenderer;                                         /**< The SDL rendering context for hardware-accelerated drawing. */
        std::unordered_map<Common::TextureID, SDL_Texture *> m_textureCache; /**< Cached textures for fast access by ID. */
    };
} // namespace Engine
