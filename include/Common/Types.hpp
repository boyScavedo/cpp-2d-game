#pragma once

#include "Common/Constants.hpp"

namespace Common
{
    // Instruction from Engine to Gameplay
    struct InputState
    {
        bool up = false;
        bool down = false;
        bool left = false;
        bool right = false;
        bool attack = false;
        bool quit = false;
    };

    // Instruction from Gameplay to Engine
    struct RenderCommand
    {
        float x = 0.0f, y = 0.0f;
        float width = 0.0f, height = 0.0f;
        Common::TextureID textureID;
    };
}