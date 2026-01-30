#include "Engine/Camera.hpp"
#include <algorithm>

/**
 * @brief Gets the current horizontal offset of the camera.
 * @return The camera's X offset in pixels.
 */
float Engine::Camera::getCameraOffsetX() const
{
    return cameraOffsetX;
}

/**
 * @brief Sets the horizontal offset of the camera.
 * @param value The new X offset value.
 */
void Engine::Camera::setCameraOffsetX(float value)
{
    cameraOffsetX = value;
}

/**
 * @brief Updates the camera's position to follow the player, clamping to world bounds.
 * @param playerX The current X position of the player.
 */
void Engine::Camera::update(float playerX)
{
    // Simple sidescrolling: camera follows player horizontally
    // Keep player centered, but clamp to world bounds
    float targetOffset = playerX - Common::VIEWPORT_WIDTH / 2.0f;
    cameraOffsetX = std::max(minCameraOffsetX, std::min(maxCameraOffsetX, targetOffset));
}
