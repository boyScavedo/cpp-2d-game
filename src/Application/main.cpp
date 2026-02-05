
#include <vector>
#include <iostream>

#include "Engine/InputManager.hpp"
#include "Engine/WindowManager.hpp"
#include "Engine/Renderer.hpp"
#include "Engine/Camera.hpp"
#include "Engine/LevelLoader.hpp"

#include "Gameplay/ECS/Registry.hpp"
#include "Gameplay/ECS/InputSystem.hpp"
#include "Gameplay/ECS/PhysicsSystem.hpp"
#include "Gameplay/ECS/RenderSystem.hpp"

#include "Common/Constants.hpp"

// Include CollisionSystem
#include "Gameplay/ECS/CollisionSystem.hpp"

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
 
     Engine::WindowManager window(Common::WINDOW_TITLE_PREFIX, Common::MINIMUM_SCREEN_WIDTH, Common::MINIMUM_SCREEN_HEIGHT);
     Engine::InputManager inputSystem;
     Engine::Renderer renderer(window.getSDLWindow());
     Engine::Camera camera;
 
     // --- ECS Initialization ---
     Gameplay::ECS::Registry registry;
     Gameplay::Systems::InputSystem ecsInput;
     Gameplay::Systems::PhysicsSystem ecsPhysics;
     Gameplay::Systems::RenderSystem ecsRender;
     Gameplay::Systems::CollisionSystem ecsCollision; // NEW
 
     // Load Level
     if (!Engine::LevelLoader::loadLevel(registry, "assets/levels/level1.json"))
     {
         std::cerr << "Failed to load initial level!" << std::endl;
         return 1;
     }

     // Load Texture Resources
     renderer.loadTexture(Common::TextureID::TEX_BACKGROUND_FAR, "assets/sprites/Background_Far.png");
     renderer.loadTexture(Common::TextureID::TEX_BACKGROUND_MID, "assets/sprites/Background_Mid_Start.png");
     renderer.loadTexture(Common::TextureID::TEX_BACKGROUND_NEAR, "assets/sprites/Background_Near_Start.png");
     renderer.loadTexture(Common::TextureID::TEX_PLAYER, "assets/sprites/Player.png");
 
     // For updating the fps counter via fpsCounter()
     Uint64 fps = 0;
     Uint64 lastFpsTime = 0;
     Uint64 lastTime = SDL_GetTicks();
 
     bool running = true;
     
     // --- Transition Logic ---
     // --- Transition Logic ---
     bool isTransitioning = false;
     float transitionAlpha = 0.0f; // 0 = Transparent, 255 = Black
     std::string pendingLevel = "";
     Gameplay::ECS::CardinalDirection entryDirection = Gameplay::ECS::CardinalDirection::East; 
     
     // Persistent State Checkpoint
     Gameplay::ECS::Physics savedPhysics;
     float savedPosY = 0.0f;
     bool hasSavedState = false;
     float currentTransitionDuration = 0.5f;

     while (running)
     {
         std::vector<Common::RenderCommand> frameCommands;
         frameCommands.reserve(32); 
 
         Uint64 currentTime = SDL_GetTicks();
         float deltaTime = (currentTime - lastTime) / 1000.0f;
         lastTime = currentTime;
 
         window.fpsCounter(currentTime, lastFpsTime, fps);
 
         if (deltaTime > 0.1f) deltaTime = 0.1f;
 
         // --- Input System ---
         Common::InputState currentInput = inputSystem.update();
         if (currentInput.quit) running = false;
         
         window.update(currentInput);
 
         // --- Game Logic ---
         if (!isTransitioning)
         {
             ecsInput.update(registry, currentInput, deltaTime);
             ecsPhysics.update(registry, deltaTime);
             
             // Check Collisions & Transitions
             auto exitData = ecsCollision.check(registry); 
             if (exitData.has_value())
             {
                 std::cout << "Level Transition Triggered! Going to: " << exitData->nextLevelPath << std::endl;
                 isTransitioning = true;
                 pendingLevel = exitData->nextLevelPath;
                 entryDirection = exitData->direction; 
                 currentTransitionDuration = exitData->transitionDuration;

                 // SAVE PLAYER STATE
                 for (const auto& [entity, physics] : registry.physics)
                 {
                     if (registry.has<Gameplay::ECS::PlayerControl>(entity) && registry.has<Gameplay::ECS::Transform>(entity))
                     {
                         savedPhysics = physics;
                         savedPosY = registry.get<Gameplay::ECS::Transform>(entity).y;
                         hasSavedState = true;
                         break;
                     }
                 }
             }
         }
         else
         {
             // FADE OUT LOGIC
             // Calculate speed based on duration (255 / duration)
             float fadeSpeed = (currentTransitionDuration > 0.0f) ? (255.0f / currentTransitionDuration) : 500.0f;
             
             transitionAlpha += fadeSpeed * deltaTime; 
             
             if (transitionAlpha >= 255.0f)
             {
                 transitionAlpha = 255.0f;
                 
                 // Perform Switch
                 registry = Gameplay::ECS::Registry(); // Clear Registry
                 if (!Engine::LevelLoader::loadLevel(registry, pendingLevel))
                 {
                     std::cerr << "Failed to load next level: " << pendingLevel << std::endl;
                     running = false; 
                 }
                 else
                 {
                    // --- SPAWN & RESTORE LOGIC ---
                    
                    // 1. Calculate X Spawn
                    float spawnX = 100.0f; 
                    if (entryDirection == Gameplay::ECS::CardinalDirection::East)
                    {
                        spawnX = 100.0f; 
                    }
                    else if (entryDirection == Gameplay::ECS::CardinalDirection::West)
                    {
                        spawnX = Common::WORLD_WIDTH - 150.0f; 
                    }
                    
                    for (const auto& [entity, control] : registry.players)
                    {
                        if (registry.has<Gameplay::ECS::Transform>(entity))
                        {
                            auto& transform = registry.get<Gameplay::ECS::Transform>(entity);
                            
                            // Apply X position
                            transform.x = spawnX;

                            // RESTORE STATE (Y-Pos + Physics)
                            if (hasSavedState && registry.has<Gameplay::ECS::Physics>(entity))
                            {
                                // Restore Vertical Position (allowing seamless jump arcs)
                                // Note: We might want to clamp this if levels have vastly different floor heights,
                                // but for now, exact Y preservation is desired.
                                transform.y = savedPosY; 
                                
                                // Restore Physics (Velocity, etc.)
                                auto& newPhysics = registry.get<Gameplay::ECS::Physics>(entity);
                                newPhysics = savedPhysics;
                            }
                            else
                            {
                                // Fallback if no state saved (first load?)
                                transform.y = Common::SCREEN_HEIGHT - transform.height;
                            }
                            
                            // Ensure camera snaps to player immediately
                            camera.update(transform.x); 
                            break; 
                        }
                    }

                    // Reset Transition
                    isTransitioning = false; 
                    transitionAlpha = 0.0f; 
                    hasSavedState = false; // Consume state
                 }
             }
         }
 
         // --- Camera Update ---
         float playerX = 0.0f;
         for (const auto& [entity, control] : registry.players)
         {
              if (registry.has<Gameplay::ECS::Transform>(entity))
              {
                  playerX = registry.get<Gameplay::ECS::Transform>(entity).x;
                  break; 
              }
         }
         camera.update(playerX);
 
         // --- Render System ---
         renderer.beginFrame();
         
         frameCommands.clear();
         ecsRender.render(registry, frameCommands);
 
          // Camera Indicator
          float indicatorWidth = 0.0f;
          if (Common::MAXIMUM_CAMERA_OFFSET_X > 0.0f)
          {
              float ratio = camera.getCameraOffsetX() / Common::MAXIMUM_CAMERA_OFFSET_X;
              if (ratio < 0.0f) ratio = 0.0f;
              if (ratio > 1.0f) ratio = 1.0f;
              indicatorWidth = ratio * 200.0f;
          }
          frameCommands.push_back({10.0f, Common::SCREEN_HEIGHT - 20.0f, indicatorWidth, 10.0f, Common::TextureID::TEX_NONE});
 
         // Draw Scene
         renderer.drawCommands(frameCommands, camera.getCameraOffsetX());

         // Draw Blackout Overlay
         if (isTransitioning)
         {
             renderer.drawOverlay(transitionAlpha);
         }
         
         renderer.endFrame();
     }
 
     return 0;
 }
