#include "Engine/Game.hpp"
#include "Engine/LevelLoader.hpp"
#include "Common/Constants.hpp"
#include <SDL3/SDL.h>

namespace Engine
{
    Game::Game()
        : m_gameState(GameState::MAIN_MENU),
          m_window(Common::WINDOW_TITLE_PREFIX, Common::MINIMUM_SCREEN_WIDTH, Common::MINIMUM_SCREEN_HEIGHT),
          m_inputManager(),
          m_renderer(m_window.getSDLWindow()),
          m_camera(),
          m_entityManager(),
          m_progressIndicator(Common::VIEWPORT_WIDTH - 200.0f - 20.0f, 20.0f, 200.0f, 10.0f),
          m_levelEditor(m_entityManager, m_camera),
          m_menuSystem(),
          m_isTransitioning(false),
          m_transitionAlpha(0.0f),
          m_pendingLevel(""),
          m_entryDirection(Gameplay::CardinalDirection::East),
          m_currentTransitionDuration(0.5f),
          m_pauseWasPressed(false),
          m_exitApp(false)
    {
    }

    Game::~Game()
    {
        // Subsystems handle their own destruction
    }

    void Game::initialize(const std::string &levelPath, bool enterEditor, float newLevelWidth)
    {
        m_levelEditor.setActive(enterEditor);
        if (enterEditor)
        {
            m_gameState = GameState::RUNNING;
        }

        if (enterEditor && levelPath.empty())
        {
            // Create a blank level for editing
            m_entityManager.levelConfig.isLeftWallClamped = true;
            m_entityManager.levelConfig.isRightWallClamped = true;
            m_entityManager.levelConfig.levelWidth = newLevelWidth;

            // Add a default player entity FIXME: NO WE ARE NOT ADDING PLAYER
            // auto player = std::make_shared<Gameplay::Player>();
            // player->transform = {50.0f, Common::SCREEN_HEIGHT - 100.0f, Common::PLAYER_WIDTH, Common::PLAYER_HEIGHT, 10};
            // player->sprite = {Common::TextureID::TEX_PLAYER, 10};
            // player->physics = {0.0f, 0.0f};
            // player->playerControl = {Common::PLAYER_MAX_SPEED};
            // player->collider = {Common::PLAYER_WIDTH, Common::PLAYER_HEIGHT, 0.0f, 0.0f, true, false};
            // m_entityManager.addEntity(player);
        }
        else if (!LevelLoader::loadLevel(m_entityManager, levelPath))
        {
            // Handle error, load default
            if (!LevelLoader::loadLevel(m_entityManager, "build/assets/levels/level1.json"))
            {
                // Error
            }
        }

        m_camera.setMaxCameraOffsetX(m_entityManager.levelConfig.levelWidth - Common::VIEWPORT_WIDTH);
        m_camera.setMinCameraOffsetX(0.0f);

        // Load Textures
        m_renderer.loadTexture(Common::TextureID::TEX_BACKGROUND_FAR, "assets/sprites/Background_Far.png");
        m_renderer.loadTexture(Common::TextureID::TEX_BACKGROUND_MID, "assets/sprites/Background_Mid_Start.png");
        m_renderer.loadTexture(Common::TextureID::TEX_BACKGROUND_NEAR, "assets/sprites/Background_Near_Start.png");
        m_renderer.loadTexture(Common::TextureID::TEX_PLAYER, "assets/sprites/Player.png");

        // Initial player state
        auto initialPlayer = m_entityManager.getPlayer();
        if (initialPlayer && !enterEditor)
        {
            // Assume user pos from auth, but for now, default
            m_camera.update(initialPlayer->transform.x);
        }
    }

    void Game::handleInput(const Common::InputState &input)
    {
        m_uiCommands.clear();

        if (input.quit)
        {
            m_gameState = GameState::EXITING;
            m_exitApp = true;
            return;
        }

        // Pause Toggle
        if (input.pause && !m_pauseWasPressed)
        {
            if (m_gameState == GameState::RUNNING)
                m_gameState = GameState::PAUSED;
            else if (m_gameState == GameState::PAUSED)
                m_gameState = GameState::RUNNING;
        }
        m_pauseWasPressed = input.pause;

        if (m_gameState == GameState::MAIN_MENU)
        {
            int menuAction = m_menuSystem.updateMainMenu(input, m_uiCommands);
            if (menuAction == 1)
            {
                m_gameState = GameState::RUNNING;
            }
            else if (menuAction == 2)
            {
                m_gameState = GameState::EXITING;
            }
        }
        else if (m_gameState == GameState::PAUSED)
        {
            int pauseAction = m_menuSystem.updatePauseMenu(input, m_uiCommands);
            if (pauseAction == 1)
                m_gameState = GameState::RUNNING;
            else if (pauseAction == 2)
                m_gameState = GameState::MAIN_MENU;
            else if (pauseAction == 3)
            {
                m_gameState = GameState::EXITING;
            }
        }
    }

    void Game::update(float deltaTime, const Common::InputState &input)
    {
        if (m_gameState == GameState::RUNNING)
        {
            if (m_levelEditor.isActive())
            {
                m_levelEditor.update(deltaTime, input);
                // Check for level editor submission
                if (hasPendingSubmission())
                {
                    m_gameState = GameState::EXITING;
                }
            }

            if (!m_isTransitioning)
            {
                m_entityManager.update(deltaTime, input);
                auto player = m_entityManager.getPlayer();
                if (player)
                    m_progressIndicator.update(player->transform.x, Common::WORLD_WIDTH);

                auto exitData = m_entityManager.checkCollisions();
                if (exitData.has_value())
                {
                    m_isTransitioning = true;
                    m_pendingLevel = exitData->nextLevelPath;
                    m_entryDirection = exitData->direction;
                    m_currentTransitionDuration = exitData->transitionDuration;
                }
            }
            else
            {
                float fadeSpeed = (m_currentTransitionDuration > 0.0f) ? (255.0f / m_currentTransitionDuration) : 500.0f;
                m_transitionAlpha += fadeSpeed * deltaTime;
                if (m_transitionAlpha >= 255.0f)
                {
                    m_entityManager.clear();
                    if (!LevelLoader::loadLevel(m_entityManager, m_pendingLevel))
                    {
                        m_gameState = GameState::EXITING;
                    }
                    else
                    {
                        m_isTransitioning = false;
                        m_transitionAlpha = 0.0f;

                        auto player = m_entityManager.getPlayer();
                        if (player)
                        {
                            if (m_entryDirection == Gameplay::CardinalDirection::East)
                            {
                                player->transform.x = 50.0f;
                            }
                            else if (m_entryDirection == Gameplay::CardinalDirection::West)
                            {
                                player->transform.x = m_entityManager.levelConfig.levelWidth - player->transform.width - 50.0f;
                            }
                            m_camera.update(player->transform.x);
                        }
                    }
                }
            }

            auto player = m_entityManager.getPlayer();
            if (player && !m_levelEditor.isActive())
                m_camera.update(player->transform.x);
        }
    }

    void Game::render()
    {
        std::vector<Common::RenderCommand> frameCommands;

        m_renderer.beginFrame();

        // Populate frameCommands
        if (m_gameState == GameState::RUNNING || m_gameState == GameState::PAUSED)
        {
            m_entityManager.render(frameCommands);
            if (m_levelEditor.isActive())
            {
                m_levelEditor.render(frameCommands);
            }
            else
            {
                m_progressIndicator.render(frameCommands);
            }
        }

        if (m_isTransitioning)
        {
            frameCommands.push_back({0, 0, (float)Common::SCREEN_WIDTH, (float)Common::SCREEN_HEIGHT,
                                     Common::TextureID::TEX_NONE, 1.0f, 0, 0, 0, (unsigned char)m_transitionAlpha});
        }

        m_renderer.drawCommands(frameCommands, m_camera.getCameraOffsetX());

        if (m_gameState == GameState::PAUSED || m_gameState == GameState::MAIN_MENU)
        {
            m_renderer.drawUI(m_uiCommands);
        }

        if (m_isTransitioning)
            m_renderer.drawOverlay(m_transitionAlpha);

        m_renderer.endFrame();

        frameCommands.clear();
        m_uiCommands.clear();
    }

    void Game::run()
    {
        Uint64 lastTime = SDL_GetTicks();

        while (isRunning())
        {
            Uint64 currentTime = SDL_GetTicks();
            float deltaTime = (currentTime - lastTime) / 1000.0f;
            lastTime = currentTime;

            Common::InputState input = m_inputManager.update(m_renderer.getSDLRenderer());
            handleInput(input);
            if (!isRunning())
                break;
            update(deltaTime, input);
            render();
        }
    }
}
