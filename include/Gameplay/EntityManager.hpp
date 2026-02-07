#pragma once

#include <vector>
#include <memory>
#include <string>
#include "Common/Types.hpp"
#include "Gameplay/Entity.hpp"
#include "Common/Constants.hpp"

namespace Gameplay
{
    struct LevelConfig
    {
        bool isLeftWallClamped = true;
        bool isRightWallClamped = true;
        float levelWidth = 2560.0f;
    };

    class EntityManager
    {
    public:
        EntityManager() = default;

        void update(float deltaTime, const Common::InputState& input);
        void render(std::vector<Common::RenderCommand>& commands);

        void addEntity(std::shared_ptr<Entity> entity);
        void removeEntity(std::shared_ptr<Entity> entity);
        void removeEntity(int entityID);
        void clear();
 
        std::shared_ptr<Player> getPlayer() const;
        const std::vector<std::shared_ptr<Entity>>& getEntities() const { return m_entities; }
        std::optional<LevelExit> checkCollisions();

        std::string toJSON() const;

        LevelConfig levelConfig;

    private:
        std::vector<std::shared_ptr<Entity>> m_entities;
        int m_nextEntityID = 0;
    };
}
