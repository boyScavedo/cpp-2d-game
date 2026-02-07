#include "Common/Constants.hpp"
#include "Gameplay/CLIHandler.hpp"
#include <iostream>

namespace Gameplay
{
    std::optional<Engine::User> CLIHandler::runAuthFlow(Engine::AuthService& authService)
    {
        std::optional<Engine::User> currentUser;
        std::cout << "=== Welcome to Scavedo Game ===" << std::endl;
        
        while (!currentUser) {
            std::cout << "1. Login" << std::endl;
            std::cout << "2. Register" << std::endl;
            std::cout << "Choice: ";
            int authChoice;
            std::cin >> authChoice;

            std::string username, password;
            if (authChoice == 1) {
                std::cout << "Username: ";
                std::cin >> username;
                std::cout << "Password: ";
                std::cin >> password;
                currentUser = authService.authenticate(username, password);
                if (!currentUser) std::cout << "Invalid credentials!" << std::endl;
            } else if (authChoice == 2) {
                std::cout << "New Username: ";
                std::cin >> username;
                std::cout << "New Password: ";
                std::cin >> password;
                if (authService.registerUser(username, password)) {
                    std::cout << "Registration successful!" << std::endl;
                    currentUser = authService.authenticate(username, password);
                } else {
                    std::cout << "Username already taken!" << std::endl;
                }
            }
        }
        return currentUser;
    }

    CLIHandler::AdminDashboardResult CLIHandler::runAdminDashboard(const Engine::User& user)
    {
        AdminDashboardResult result;
        if (user.role != Engine::UserRole::Admin) return result;

        std::cout << "\nWelcome Admin " << user.username << "!" << std::endl;
        std::cout << "1. Go to Admin Dashboard (List Levels)" << std::endl;
        std::cout << "2. Create New Level" << std::endl;
        std::cout << "3. Start Game (Current Progress)" << std::endl;
        std::cout << "4. Logout & Exit" << std::endl;
        std::cout << "Choice: ";
        int choice;
        std::cin >> choice;

        if (choice == 1) {
            std::cout << "\n=============================================" << std::endl;
            std::cout << "           ADMIN LEVEL DASHBOARD             " << std::endl;
            std::cout << "=============================================" << std::endl;
            std::cout << "\n[CAMPAIGN LEVELS]" << std::endl;
            std::cout << " - assets/levels/level1.json" << std::endl;
            std::cout << " - assets/levels/level2.json" << std::endl;
            
            std::cout << "\n[CUSTOM LEVELS]" << std::endl;
            std::cout << " - (No custom levels found)" << std::endl;

            std::cout << "\n[EDITOR CONTROLS]" << std::endl;
            std::cout << " C         : Create New Object" << std::endl;
            std::cout << " Backspace : Delete Selected Object" << std::endl;
            std::cout << " WASD      : Move Object / Pan View" << std::endl;
            std::cout << " Shift+WASD: Resize/Scale Object" << std::endl;
            std::cout << " ENTER     : Submit Level & Save to JSON" << std::endl;
            
            std::cout << "\n---------------------------------------------" << std::endl;
            std::cout << "Enter level path to LOAD or press ENTER for default: ";
            std::cin.ignore();
            std::getline(std::cin, result.levelToLoad);
            
            result.enterEditor = true;
            return result;
        } else if (choice == 2) {
            result.enterEditor = true;
            result.createNewLevel = true;
            std::cout << "Enter New Level Width (Default " << Common::WORLD_WIDTH << "): ";
            std::string widthInput;
            std::cin.ignore();
            std::getline(std::cin, widthInput);
            if (!widthInput.empty()) {
                result.newLevelWidth = std::stof(widthInput);
            }

            std::cout << "\n[EDITOR CONTROLS ACTIVE]" << std::endl;
            std::cout << " C         : Create New Object" << std::endl;
            std::cout << " Backspace : Delete Selected Object" << std::endl;
            std::cout << " WASD      : Move Object / Pan View" << std::endl;
            std::cout << " Shift+WASD: Resize/Scale Object" << std::endl;
            std::cout << " ENTER     : Submit Level & Save to JSON" << std::endl;
            std::cout << "---------------------------------------------" << std::endl;

            return result;
        } else if (choice == 4) {
            result.shouldExitApp = true;
            return result;
        }

        return result;
    }
}
