#pragma once

#include "Common/Constants.hpp"

namespace Engine
{
    /**
     * @class Camera
     * @brief Manages the camera's position and offset for sidescrolling gameplay.
     *
     * The Camera class is responsible for creating a smooth, player-following camera system
     * essential for 2D platformers. It maintains the camera's horizontal offset within world boundaries,
     * ensuring the player stays visible while allowing exploration. This class works closely with
     * the Renderer to apply camera transformations during drawing, enabling parallax effects and
     * dynamic viewport management. The camera's behavior supports level design by clamping to
     * configurable min/max bounds, preventing out-of-bounds rendering.
     */
    class Camera
    {
    private:
        float cameraOffsetX = 0.0f;       /**< Current horizontal offset of the camera in pixels. */
        float minCameraOffsetX = 0.0f;    /**< Minimum allowed camera offset to prevent scrolling left beyond world start. */
        float maxCameraOffsetX = 2560.0f; /**< Maximum allowed camera offset to prevent scrolling right beyond world end. */

    public:
        /**
         * @brief Gets the minimum allowed camera offset on the X-axis.
         * @return The minimum camera offset value.
         */
        float getMinCameraOffsetX() const { return minCameraOffsetX; }

        /**
         * @brief Gets the maximum allowed camera offset on the X-axis.
         * @return The maximum camera offset value.
         */
        float getMaxCameraOffsetX() const { return maxCameraOffsetX; }

        /**
         * @brief Sets the minimum allowed camera offset on the X-axis.
         * @param value The new minimum offset value, typically 0.0f for world start.
         */
        void setMinCameraOffsetX(float value) { minCameraOffsetX = value; }

        /**
         * @brief Sets the maximum allowed camera offset on the X-axis.
         * @param value The new maximum offset value, usually levelWidth - viewportWidth.
         */
        void setMaxCameraOffsetX(float value) { maxCameraOffsetX = value; }

        /**
         * @brief Gets the current horizontal offset of the camera.
         * @return The camera's X offset in pixels, used by Renderer for drawing transformations.
         */
        float getCameraOffsetX() const;

        /**
         * @brief Sets the horizontal offset of the camera directly.
         * @param value The new X offset value; will be clamped to min/max bounds.
         */
        void setCameraOffsetX(float value);

        /**
         * @brief Updates the camera's position to follow the player, clamping to world bounds.
         *
         * Centers the camera on the player's X position while respecting min/max offset limits.
         * This creates smooth camera movement that keeps the player in view during exploration.
         * Called each frame with the player's current position.
         *
         * @param playerX The current X position of the player in world coordinates.
         */
        void update(float playerX);
    };
}
