/**
 * @file main.cpp
 * @brief Application entry point and main game loop implementation.
 *
 * This file contains the high-level game architecture, orchestrating the interaction between
 * different subsystems. The game follows a layered architecture:
 *
 * - Engine Layer: Core systems (Window, Renderer, Input, Camera, Auth)
 * - Gameplay Layer: Game logic (Entities, Components, EntityManager, UI)
 * - Admin Layer: Editor tools and level creation
 *
 * The main function implements a dual-mode application:
 * 1. CLI Mode: Authentication and admin dashboard for setup/level editing
 * 2. Graphical Mode: Main game loop with rendering, physics, and input processing
 *
 * The game loop uses a state machine pattern with GameState enum to manage different
 * application states (Main Menu, Running, Paused, Exiting). Level transitions are
 * handled through collision detection with LevelExit entities, supporting seamless
 * world progression.
 */

#include <vector>
#include <iostream>
#include <fstream>

#include "Engine/Game.hpp"
#include "Engine/AuthService.hpp"
#include "Gameplay/CLIHandler.hpp"

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

    if (!currentUser)
        return 1;

    bool appRunning = true;
    while (appRunning)
    {
        Gameplay::CLIHandler::AdminDashboardResult adminResult = Gameplay::CLIHandler::runAdminDashboard(*currentUser);

        if (adminResult.shouldExitApp)
        {
            appRunning = false;
            break;
        }

        // Scope-based Engine Initialization (Window destroyed when we return to dashboard)
        {
            std::string levelPath = adminResult.createNewLevel ? "" : (adminResult.levelToLoad.empty() ? "assets/levels/level1.json" : "assets/levels/" + adminResult.levelToLoad);
            Engine::Game game;
            game.initialize(levelPath, adminResult.enterEditor, adminResult.newLevelWidth);
            game.run();

            // Handle level editor submission
            if (game.hasPendingSubmission())
            {
                auto submission = game.getPendingSubmission();
                std::cout << "Level submission detected. Enter level name (without .json): ";
                std::string levelName;
                std::cin >> levelName;

                // Save the level to JSON
                std::string jsonData = game.getEntityManagerJSON();

                // Save to build/assets/levels/
                std::string buildFilePath = "build/assets/levels/" + levelName + ".json";
                std::ofstream buildOutFile(buildFilePath);
                if (buildOutFile.is_open())
                {
                    buildOutFile << jsonData;
                    buildOutFile.close();
                    std::cout << "Level saved successfully to build/assets/levels/ as " << levelName << ".json" << std::endl;
                }
                else
                {
                    std::cout << "Failed to save level to " << buildFilePath << std::endl;
                }

                // Save to assets/levels/
                std::string assetsFilePath = "assets/levels/" + levelName + ".json";
                std::ofstream assetsOutFile(assetsFilePath);
                if (assetsOutFile.is_open())
                {
                    assetsOutFile << jsonData;
                    assetsOutFile.close();
                    std::cout << "Level saved successfully to assets/levels/ as " << levelName << ".json" << std::endl;
                }
                else
                {
                    std::cout << "Failed to save level to " << assetsFilePath << std::endl;
                }
            }

            if (game.shouldExitApp())
                appRunning = false;
        }
    }

    return 0;
}
