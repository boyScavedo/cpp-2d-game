#include "Common/Types.hpp"
#include "Gameplay/Entity.hpp"
#include <iostream>

namespace Gameplay
{
    // --- Player Implementation ---
    Player::Player()
    {
        playerControl = PlayerControl();
        physics = Physics();
    }

    void Player::handleInput(const Common::InputState& input)
    {
        if (!playerControl || !physics) return;

        // Reset horizontal acceleration
        float accelerationX = 0.0f;

        if (input.left) {
            accelerationX = -playerControl->speed;
        } else if (input.right) {
            accelerationX = playerControl->speed;
        }

        // Apply horizontal movement and friction
        if (accelerationX != 0.0f) {
            physics->velocityX = accelerationX;
        } else {
            // Decelerate
            if (physics->velocityX > 0) {
                physics->velocityX = std::max(0.0f, physics->velocityX - physics->friction * 500.0f * 0.016f); // Approximation
            } else if (physics->velocityX < 0) {
                physics->velocityX = std::min(0.0f, physics->velocityX + physics->friction * 500.0f * 0.016f);
            }
        }

        // Clamp Speed
        if (physics->velocityX > physics->maxSpeed) physics->velocityX = physics->maxSpeed;
        if (physics->velocityX < -physics->maxSpeed) physics->velocityX = -physics->maxSpeed;

        // Jump Logic
        if (input.up && physics->isGrounded && playerControl->canJump) {
            physics->velocityY = Common::JUMP_FORCE;
            physics->isGrounded = false;
        }
    }

    void Player::update(float deltaTime)
    {
        if (!physics) return;

        // Apply Gravity
        physics->velocityY += physics->gravity * deltaTime;

        // Terminal Velocity
        if (physics->velocityY > Common::TERMINAL_VELOCITY) physics->velocityY = Common::TERMINAL_VELOCITY;

        // Apply Velocity to Position
        transform.x += physics->velocityX * deltaTime;
        transform.y += physics->velocityY * deltaTime;
    }

    void Player::render(std::vector<Common::RenderCommand>& commands)
    {
        if (sprite) {
            commands.push_back({
                .x = transform.x,
                .y = transform.y,
                .width = transform.width,
                .height = transform.height,
                .textureID = sprite->textureID,
                .scrollFactor = 1.0f
            });
        }
    }

    // --- StaticObject Implementation ---
    void StaticObject::render(std::vector<Common::RenderCommand>& commands)
    {
        if (sprite) {
            commands.push_back({
                .x = transform.x,
                .y = transform.y,
                .width = transform.width,
                .height = transform.height,
                .textureID = sprite->textureID,
                .scrollFactor = 1.0f
            });
        }
    }

    // --- BackgroundLayer Implementation ---
    void BackgroundLayer::render(std::vector<Common::RenderCommand>& commands)
    {
        if (sprite) {
            commands.push_back({
                .x = transform.x,
                .y = transform.y,
                .width = transform.width,
                .height = transform.height,
                .textureID = sprite->textureID,
                .scrollFactor = parallax ? parallax->factor : 1.0f
            });
        }
    }
}
