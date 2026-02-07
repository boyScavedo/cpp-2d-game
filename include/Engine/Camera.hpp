#pragma once

#include "Common/Constants.hpp"

namespace Engine
{
    /**
     * @class Camera
     * @brief Manages the camera's position and offset for sidescrolling gameplay.
     *
     * The Camera class tracks the horizontal offset of the camera, ensuring it follows the player
     * while staying within the defined world bounds.
     */
    class Camera
    {
    private:
        float cameraOffsetX = 0.0f;                               
        float minCameraOffsetX = 0.0f; 
        float maxCameraOffsetX = 2560.0f; 
    public:
        float getMinCameraOffsetX() const { return minCameraOffsetX; }
        float getMaxCameraOffsetX() const { return maxCameraOffsetX; }
        void setMinCameraOffsetX(float value) { minCameraOffsetX = value; }
        void setMaxCameraOffsetX(float value) { maxCameraOffsetX = value; }
        /**
         * @brief Gets the current horizontal offset of the camera.
         * @return The camera's X offset in pixels.
         */
        float getCameraOffsetX() const;

        /**
         * @brief Sets the horizontal offset of the camera.
         * @param value The new X offset value.
         */
        void setCameraOffsetX(float value);

        /**
         * @brief Updates the camera's position to follow the player, clamping to world bounds.
         * @param playerX The current X position of the player.
         */
        void update(float playerX);
    };
}
