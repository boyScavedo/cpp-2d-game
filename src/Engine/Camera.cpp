/**
 * @file Camera.cpp
 * @brief Implementation of the Camera class for managing viewport scrolling.
 *
 * This file provides the concrete implementation of camera movement logic used in
 * sidescrolling gameplay. The camera smoothly follows the player while respecting
 * world boundaries to prevent showing areas outside the level.
 */

#include "Engine/Camera.hpp"
#include <algorithm>

/**
 * @brief Gets the current horizontal offset of the camera.
 *
 * Returns the camera's X offset value, which represents how much the viewport
 * has scrolled horizontally. This value is used by the renderer to offset all
 * world coordinates during drawing.
 *
 * @return The camera's X offset in pixels.
 */
float Engine::Camera::getCameraOffsetX() const
{
    return cameraOffsetX;
}

/**
 * @brief Sets the horizontal offset of the camera directly.
 *
 * Allows direct manipulation of the camera position, useful for initialization
 * or programmatic camera control. The value is not clamped automatically here,
 * so callers should ensure it stays within valid bounds.
 *
 * @param value The new X offset value in pixels.
 */
void Engine::Camera::setCameraOffsetX(float value)
{
    cameraOffsetX = value;
}

/**
 * @brief Updates the camera's position to follow the player, clamping to world bounds.
 *
 * Implements smooth sidescrolling camera behavior by centering the player in the viewport.
 * The camera calculates a target offset that would place the player at the center of the screen,
 * then clamps this value to stay within the defined world boundaries (minCameraOffsetX to maxCameraOffsetX).
 * This prevents the camera from showing areas outside the level or going beyond the world's edges.
 *
 * @param playerX The current X position of the player in world coordinates.
 */
void Engine::Camera::update(float playerX)
{
    // Simple sidescrolling: camera follows player horizontally
    // Keep player centered, but clamp to world bounds
    float targetOffset = playerX - Common::VIEWPORT_WIDTH / 2.0f;
    cameraOffsetX = std::max(minCameraOffsetX, std::min(maxCameraOffsetX, targetOffset));
}
