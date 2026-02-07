/**
 * @file main.cpp
 * @brief Application entry point and main game loop implementation.
 */

#include <vector>
#include <iostream>
#include <fstream>

#include "Engine/InputManager.hpp"
#include "Engine/WindowManager.hpp"
#include "Engine/Renderer.hpp"
#include "Engine/Camera.hpp"
#include "Engine/LevelLoader.hpp"

#include "Engine/AuthService.hpp"
#include "Admin/LevelEditor.hpp"
#include "Gameplay/CLIHandler.hpp"
#include "Gameplay/UI/ProgressIndicator.hpp"
#include "Gameplay/EntityManager.hpp"
#include "Gameplay/Entity.hpp"
#include "Gameplay/Components.hpp"

 /**
  * @brief Application entry point that initializes engine subsystems and runs the main game loop.
  *
  * Initializes the window, input manager, renderer, and ECS Registry.
  * Loads the initial level from JSON and enters the game loop.
  *
  * @return int Exit code; `0` indicates successful termination.
  */
 int main(int argc, char *argv[])
 {
     // Suppress unused parameter warnings
     (void)argc;
     (void)argv;
 
     // --- Authentication System ---
     Engine::AuthService authService;
     std::optional<Engine::User> currentUser = Gameplay::CLIHandler::runAuthFlow(authService);
 
     if (!currentUser) return 1;

     bool appRunning = true;
     while (appRunning) {
         Gameplay::CLIHandler::AdminDashboardResult adminResult = Gameplay::CLIHandler::runAdminDashboard(*currentUser);
         
         if (adminResult.shouldExitApp) {
             appRunning = false;
             break;
         }

         // Scope-based Engine Initialization (Window destroyed when we return to dashboard)
         {
             Engine::WindowManager window(Common::WINDOW_TITLE_PREFIX, Common::MINIMUM_SCREEN_WIDTH, Common::MINIMUM_SCREEN_HEIGHT);
             Engine::InputManager inputSystem;
             Engine::Renderer renderer(window.getSDLWindow());
             Engine::Camera camera;
             Gameplay::UI::ProgressIndicator progressIndicator(Common::VIEWPORT_WIDTH - 200.0f - 20.0f, 20.0f, 200.0f, 10.0f);
             Gameplay::EntityManager entityManager;
             Admin::LevelEditor levelEditor(entityManager, camera);

             if (adminResult.enterEditor) {
                 levelEditor.setActive(true);
             }

             if (adminResult.createNewLevel) {
                 std::cout << "Initializing fresh level (Width: " << adminResult.newLevelWidth << ")..." << std::endl;
                 entityManager.levelConfig.isLeftWallClamped = true;
                 entityManager.levelConfig.isRightWallClamped = true;
                 entityManager.levelConfig.levelWidth = adminResult.newLevelWidth;
                 // Note: No player is spawned in a fresh level for the editor
             } else {
                 std::string levelPath = adminResult.levelToLoad.empty() ? currentUser->currentLevel : adminResult.levelToLoad;
                 if (!Engine::LevelLoader::loadLevel(entityManager, levelPath)) {
                     std::cerr << "Failed to load level: " << levelPath << "!" << std::endl;
                     if (!Engine::LevelLoader::loadLevel(entityManager, "build/assets/levels/level1.json")) {
                         appRunning = false;
                         break;
                     }
                 }
             }

             camera.setMaxCameraOffsetX(entityManager.levelConfig.levelWidth - Common::VIEWPORT_WIDTH);
             camera.setMinCameraOffsetX(0.0f);

             // Load Textures
             renderer.loadTexture(Common::TextureID::TEX_BACKGROUND_FAR, "assets/sprites/Background_Far.png");
             renderer.loadTexture(Common::TextureID::TEX_BACKGROUND_MID, "assets/sprites/Background_Mid_Start.png");
             renderer.loadTexture(Common::TextureID::TEX_BACKGROUND_NEAR, "assets/sprites/Background_Near_Start.png");
             renderer.loadTexture(Common::TextureID::TEX_PLAYER, "assets/sprites/Player.png");

             // Initial player state for non-new levels
             auto initialPlayer = entityManager.getPlayer();
             if (initialPlayer && !adminResult.enterEditor) {
                 if (currentUser->posX > 0.0f) {
                     initialPlayer->transform.x = currentUser->posX;
                     initialPlayer->transform.y = currentUser->posY;
                 }
                 camera.update(initialPlayer->transform.x);
             }

             Uint64 lastTime = SDL_GetTicks();
             Uint64 lastFpsTime = 0, fps = 0;
             bool running = true;
             bool isTransitioning = false;
             float transitionAlpha = 0.0f;
             std::string pendingLevel = "";
             Gameplay::CardinalDirection entryDirection = Gameplay::CardinalDirection::East;
             float currentTransitionDuration = 0.5f;

             while (running) {
                 std::vector<Common::RenderCommand> frameCommands;
                 Uint64 currentTime = SDL_GetTicks();
                 float deltaTime = (currentTime - lastTime) / 1000.0f;
                 lastTime = currentTime;
                 if (deltaTime > 0.1f) deltaTime = 0.1f;

                 window.fpsCounter(currentTime, lastFpsTime, fps);
                 Common::InputState input = inputSystem.update();
                 if (input.quit) {
                     running = false;
                     appRunning = false;
                 }
                 window.update(input);

                 if (levelEditor.isActive()) {
                     levelEditor.update(deltaTime, input);
                 }

                 if (!isTransitioning) {
                     entityManager.update(deltaTime, input);
                     auto player = entityManager.getPlayer();
                     if (player) progressIndicator.update(player->transform.x, Common::WORLD_WIDTH);

                     auto exitData = entityManager.checkCollisions();
                     if (exitData.has_value()) {
                         isTransitioning = true;
                         pendingLevel = exitData->nextLevelPath;
                         entryDirection = exitData->direction;
                         currentTransitionDuration = exitData->transitionDuration;
                     }
                 } else {
                     float fadeSpeed = (currentTransitionDuration > 0.0f) ? (255.0f / currentTransitionDuration) : 500.0f;
                     transitionAlpha += fadeSpeed * deltaTime;
                     if (transitionAlpha >= 255.0f) {
                         entityManager.clear();
                         if (!Engine::LevelLoader::loadLevel(entityManager, pendingLevel)) {
                             running = false;
                         } else {
                             isTransitioning = false;
                             transitionAlpha = 0.0f;
                             
                             auto player = entityManager.getPlayer();
                             if (player) {
                                 if (entryDirection == Gameplay::CardinalDirection::East) {
                                     player->transform.x = 50.0f; // Arrive from West
                                 } else if (entryDirection == Gameplay::CardinalDirection::West) {
                                     player->transform.x = entityManager.levelConfig.levelWidth - player->transform.width - 50.0f; // Arrive from East
                                 }
                                 camera.update(player->transform.x);
                             }
                         }
                     }
                 }

                 auto submission = levelEditor.getPendingSubmission();
                 if (submission) {
                     std::cout << "\n--- Level Submission ---" << std::endl;
                     
                     // Connection Direction Selection
                     std::cout << "Where will this level be connected?" << std::endl;
                     std::cout << "1. East" << std::endl;
                     std::cout << "2. West" << std::endl;
                     std::cout << "3. North" << std::endl;
                     std::cout << "4. South" << std::endl;
                     std::cout << "5. That's it (None)" << std::endl;
                     std::cout << "Choice: ";
                     int dirChoice = 5;
                     std::cin >> dirChoice;

                     if (dirChoice == 1) entityManager.levelConfig.isRightWallClamped = false;
                     if (dirChoice == 2) entityManager.levelConfig.isLeftWallClamped = false;

                     // 1. Ensure Player exists
                     if (!entityManager.getPlayer()) {
                         auto player = std::make_shared<Gameplay::Player>();
                         player->transform = {200, 300, 50, 50, 10}; 
                         player->sprite = {Common::TextureID::TEX_PLAYER, 10};
                         player->physics = {0, 0};
                         player->playerControl = {600.0f};
                         player->collider = {50, 50, 0, 0, false, true};
                         entityManager.addEntity(player);
                     }

                     // 2. Automatically create LevelExit to level1.json on unclamped sides
                     if (dirChoice >= 1 && dirChoice <= 4) {
                         auto exitObj = std::make_shared<Gameplay::StaticObject>();
                         exitObj->transform.width = 100.0f;
                         exitObj->transform.height = 2000.0f; // Tall trigger
                         exitObj->transform.y = -500.0f;
                         exitObj->transform.zIndex = 0;

                         Gameplay::LevelExit exitComp;
                         exitComp.nextLevelPath = "assets/levels/level1.json";
                         exitComp.transitionDuration = 0.5f;

                         if (dirChoice == 1) { // East
                             exitObj->transform.x = entityManager.levelConfig.levelWidth;
                             exitComp.direction = Gameplay::CardinalDirection::East;
                         } else if (dirChoice == 2) { // West
                             exitObj->transform.x = -100.0f;
                             exitComp.direction = Gameplay::CardinalDirection::West;
                         } else if (dirChoice == 3) { // North
                             exitObj->transform.y = -100.0f;
                             exitObj->transform.height = 100.0f;
                             exitObj->transform.width = entityManager.levelConfig.levelWidth;
                             exitComp.direction = Gameplay::CardinalDirection::North;
                         } else if (dirChoice == 4) { // South
                             exitObj->transform.y = 1000.0f; // Arbitrary high value
                             exitObj->transform.height = 100.0f;
                             exitObj->transform.width = entityManager.levelConfig.levelWidth;
                             exitComp.direction = Gameplay::CardinalDirection::South;
                         }

                         exitObj->exit = exitComp;
                         exitObj->collider = {exitObj->transform.width, exitObj->transform.height, 0, 0, true, false}; // Trigger
                         entityManager.addEntity(exitObj);
                         std::cout << "Added auto-teleport to level1.json for choice " << dirChoice << std::endl;
                     }

                     std::string name;
                     std::cout << "Level Name (e.g., custom_level1): ";
                     std::cin >> name;
                     std::string filename = "assets/levels/" + name + ".json";
                     std::string buildFilename = "build/assets/levels/" + name + ".json";
                     
                     std::string jsonData = entityManager.toJSON();
                     
                     auto saveFile = [](const std::string& path, const std::string& data) {
                         std::ofstream file(path);
                         if (file.is_open()) {
                             file << data;
                             return true;
                         }
                         return false;
                     };

                     if (saveFile(filename, jsonData)) {
                         std::cout << "Level saved to " << filename << std::endl;
                         if (saveFile(buildFilename, jsonData)) {
                             std::cout << "Build assets synced: " << buildFilename << std::endl;
                         }
                         running = false; 
                     }
                 }

                 auto player = entityManager.getPlayer();
                 if (player && !levelEditor.isActive()) camera.update(player->transform.x);

                 renderer.beginFrame();
                 frameCommands.clear();
                 entityManager.render(frameCommands);
                 progressIndicator.render(frameCommands);
                 levelEditor.render(frameCommands);
                 renderer.drawCommands(frameCommands, camera.getCameraOffsetX());
                 if (isTransitioning) renderer.drawOverlay(transitionAlpha);
                 renderer.endFrame();
             }
         } // SDL Window destroyed here
     }

     return 0;
 }
