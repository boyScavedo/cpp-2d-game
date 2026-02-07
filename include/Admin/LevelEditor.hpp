#pragma once

#include "Engine/Camera.hpp"
#include "Gameplay/EntityManager.hpp"
#include "Common/Types.hpp"
#include <memory>

namespace Admin
{
    class LevelEditor
    {
    public:
        LevelEditor(Gameplay::EntityManager& entityManager, Engine::Camera& camera);

        void update(float deltaTime, const Common::InputState& input);
        void render(std::vector<Common::RenderCommand>& commands);

        bool isActive() const { return m_active; }
        void setActive(bool active) { m_active = active; }

        struct Suggestion {
            std::string levelName;
            std::string connectionPath;
            std::string leadPath;
        };

        std::optional<Suggestion> getPendingSubmission() { 
            auto s = m_pendingSubmission; 
            m_pendingSubmission = std::nullopt; 
            return s; 
        }

    private:
        Gameplay::EntityManager& m_entityManager;
        Engine::Camera& m_camera;
        std::shared_ptr<Gameplay::Entity> m_selectedEntity;
        bool m_active = false;

        std::optional<Suggestion> m_pendingSubmission;

        void handleObjectManipulation(float deltaTime, const Common::InputState& input);
        void createDefaultObject();
        void deleteSelectedObject();
    };
}
