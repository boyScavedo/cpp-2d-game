#pragma once

#include "Engine/AuthService.hpp"
#include <optional>
#include <string>

namespace Gameplay
{
    /**
     * @class CLIHandler
     * @brief Manages CLI-based user interactions such as authentication and admin dashboards.
     */
    class CLIHandler
    {
    public:
        /**
         * @brief Runs the login/registration flow.
         * @param authService The service to use for authentication logic.
         * @return std::optional<Engine::User> The authenticated user, if successful.
         */
        static std::optional<Engine::User> runAuthFlow(Engine::AuthService& authService);

        /**
         * @struct AdminDashboardResult
         * @brief Represents the choices made by an admin in the dashboard.
         */
        struct AdminDashboardResult {
            bool enterEditor = false;
            bool createNewLevel = false;
            float newLevelWidth = 2560.0f;
            std::string levelToLoad = "";
            bool shouldExitApp = false;
        };

        /**
         * @brief Runs the admin dashboard CLI UI.
         * @param user The authenticated admin user.
         * @return AdminDashboardResult The admin's dashboard selections.
         */
        static AdminDashboardResult runAdminDashboard(const Engine::User& user);
    };
}
