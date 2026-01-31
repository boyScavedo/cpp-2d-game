#pragma once

#include "Common/Constants.hpp"
#include "Common/Types.hpp"

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
        float cameraOffsetX = 0.0f;                               /**< The current horizontal offset of the camera. */
        float minCameraOffsetX = Common::MINIMUM_CAMERA_OFFSET_X; /**< The minimum allowed camera offset. */
        float maxCameraOffsetX = Common::MAXIMUM_CAMERA_OFFSET_X; /**< The maximum allowed camera offset. */

    public:
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
