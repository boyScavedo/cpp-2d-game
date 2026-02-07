#pragma once

#include <vector>
#include <memory>
#include <optional>
#include "Common/Types.hpp"
#include "Gameplay/Components.hpp"
#include "Common/Constants.hpp"

namespace Gameplay
{
    class Entity
    {
    public:
        Entity() = default;
        virtual ~Entity() = default;

        virtual void update(float deltaTime) = 0;
        virtual void render(std::vector<Common::RenderCommand>& commands) = 0;

        Transform transform;
        std::optional<Sprite> sprite;
        std::optional<Physics> physics;
        std::optional<Collider> collider;
        std::optional<LevelExit> exit;
        std::optional<PlayerControl> playerControl;
        std::optional<Parallax> parallax;

        int id = -1;
        bool isDestroyed = false;
    };

    class Player : public Entity
    {
    public:
        Player();
        void update(float deltaTime) override;
        void render(std::vector<Common::RenderCommand>& commands) override;
        
        void handleInput(const Common::InputState& input);
    };

    class StaticObject : public Entity
    {
    public:
        StaticObject() = default;
        void update([[maybe_unused]] float deltaTime) override {}
        void render(std::vector<Common::RenderCommand>& commands) override;
    };
    
    class BackgroundLayer : public Entity
    {
    public:
        BackgroundLayer() = default;
        void update([[maybe_unused]] float deltaTime) override {}
        void render(std::vector<Common::RenderCommand>& commands) override;
    };
}
