#pragma once

#include "Engine/AuthService.hpp"
#include <optional>
#include <string>

namespace Gameplay
{
    /**
     * @class CLIHandler
     * @brief Manages command-line interface (CLI) interactions for user authentication and administrative functions.
     *
     * The CLIHandler serves as the bridge between the game's core engine and user-facing text-based interfaces.
     * It handles the initial authentication flow (login/registration) that occurs before graphical gameplay begins,
     * and provides administrative tools for level editing and game management. This class is crucial for the
     * game's dual-mode operation: CLI for setup and administration, graphical for actual gameplay. It integrates
     * with AuthService for user management and returns structured results that drive the main application flow,
     * enabling role-based access to features like the level editor.
     */
    class CLIHandler
    {
    public:
        /**
         * @brief Runs the complete authentication flow (login or registration) via CLI.
         *
         * Presents a text-based menu allowing users to either log in with existing credentials
         * or register a new account. This method handles input validation, communicates with
         * the AuthService for credential verification, and supports both Player and Admin roles.
         * Successful authentication returns user data for session management; failure allows retry.
         * This is the entry point for user identification before entering graphical modes.
         *
         * @param authService Reference to the authentication service for user validation and registration.
         * @return std::optional<Engine::User> Authenticated user data if successful, empty optional on failure or exit.
         */
        static std::optional<Engine::User> runAuthFlow(Engine::AuthService &authService);

        /**
         * @struct AdminDashboardResult
         * @brief Encapsulates the outcome of an admin's dashboard interaction.
         *
         * This structure communicates the admin's choices back to the main application,
         * determining the next operational mode (editor, new level creation, level loading, or exit).
         * It supports flexible level management workflows, allowing admins to create custom content
         * or load existing levels for editing/playback.
         */
        struct AdminDashboardResult
        {
            bool enterEditor = false;      /**< True if admin chose to enter the level editor mode. */
            bool createNewLevel = false;   /**< True if admin wants to create a new blank level. */
            float newLevelWidth = 2560.0f; /**< Width of the new level to create (in pixels). */
            std::string levelToLoad = "";  /**< Path/name of existing level to load for editing or play. */
            bool shouldExitApp = false;    /**< True if admin chose to exit the application entirely. */
        };

        /**
         * @brief Runs the administrative dashboard interface via CLI.
         *
         * Displays a menu of options available to admin users, including level editing tools,
         * new level creation, and level selection. This method collects the admin's preferences
         * and returns them in a structured format for the main application to process.
         * It ensures admins have full control over content creation and game configuration.
         *
         * @param user The authenticated admin user (role verification assumed by caller).
         * @return AdminDashboardResult The admin's selected options and configuration choices.
         */
        static AdminDashboardResult runAdminDashboard(const Engine::User &user);
    };
}
