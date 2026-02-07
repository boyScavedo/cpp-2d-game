/**
 * @file Camera.cpp
 * @brief Implementation of the Camera class for managing viewport scrolling.
 */

#include "Engine/Camera.hpp"
#include <algorithm>

float Engine::Camera::getCameraOffsetX() const
{
    return cameraOffsetX;
}

void Engine::Camera::setCameraOffsetX(float value)
{
    cameraOffsetX = value;
}

void Engine::Camera::update(float playerX)
{
    // Simple sidescrolling: camera follows player horizontally
    // Keep player centered, but clamp to world bounds
    float targetOffset = playerX - Common::VIEWPORT_WIDTH / 2.0f;
    cameraOffsetX = std::max(minCameraOffsetX, std::min(maxCameraOffsetX, targetOffset));
}
