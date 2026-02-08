#pragma once

#include "Engine/WindowManager.hpp"
#include "Engine/InputManager.hpp"
#include "Engine/Renderer.hpp"
#include "Engine/Camera.hpp"
#include "Engine/AuthService.hpp"
#include "Gameplay/EntityManager.hpp"
#include "Gameplay/UI/MenuSystem.hpp"
#include "Gameplay/UI/ProgressIndicator.hpp"
#include "Admin/LevelEditor.hpp"
#include "Common/Types.hpp"
#include <optional>

namespace Engine
{
    class Game
    {
    public:
        enum class GameState
        {
            MAIN_MENU,
            RUNNING,
            PAUSED,
            EXITING
        };

        Game();
        ~Game();

        void initialize(const std::string &levelPath, bool enterEditor, float newLevelWidth = 2560.0f);
        void handleInput(const Common::InputState &input);
        void update(float deltaTime, const Common::InputState &input);
        void render();
        void run();

        bool isRunning() const { return m_gameState != GameState::EXITING; }
        bool shouldExitApp() const { return m_exitApp; }
        bool hasPendingSubmission() const { return m_levelEditor.hasPendingSubmission(); }
        std::optional<Admin::LevelEditor::Suggestion> getPendingSubmission() { return m_levelEditor.getPendingSubmission(); }
        std::string getEntityManagerJSON() const { return m_entityManager.toJSON(); }

    private:
        GameState m_gameState;
        WindowManager m_window;
        InputManager m_inputManager;
        Renderer m_renderer;
        Camera m_camera;
        Gameplay::EntityManager m_entityManager;
        Gameplay::UI::ProgressIndicator m_progressIndicator;
        Admin::LevelEditor m_levelEditor;
        Gameplay::UI::MenuSystem m_menuSystem;

        // Transition state
        bool m_isTransitioning;
        float m_transitionAlpha;
        std::string m_pendingLevel;
        Gameplay::CardinalDirection m_entryDirection;
        float m_currentTransitionDuration;

        // Input debouncing
        bool m_pauseWasPressed;

        // Exit flag
        bool m_exitApp;

        // UI commands
        std::vector<Common::RenderCommand> m_uiCommands;
    };
}
