#pragma once

#include <string>
#include <vector>
#include <optional>

namespace Engine
{
    enum class UserRole
    {
        Player,
        Admin
    };

    struct User
    {
        std::string username;
        UserRole role;
        std::string currentLevel;
        float posX = 0.0f;
        float posY = 0.0f;
    };

    class AuthService
    {
    public:
        AuthService();
        
        std::optional<User> authenticate(const std::string& username, const std::string& password);
        bool registerUser(const std::string& username, const std::string& password, UserRole role = UserRole::Player);
        void updateProgress(const std::string& username, const std::string& level, float x, float y);
        
    private:
        struct UserData {
            std::string username;
            std::string password;
            UserRole role;
            std::string currentLevel;
            float posX;
            float posY;
        };
        std::vector<UserData> m_users;
        const std::string m_filePath = "build/assets/users/users.json";

        void loadUsers();
        void saveUsers();
        std::string obfuscate(const std::string& data);
    };
}
