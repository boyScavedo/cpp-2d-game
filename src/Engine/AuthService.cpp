#include <iostream>
#include <fstream>
#include "Engine/AuthService.hpp"
#include "nlohmann/json.hpp"

using json = nlohmann::json;

namespace Engine
{
    AuthService::AuthService()
    {
        loadUsers();
        if (m_users.empty()) {
            // Initial mock data if no file exists
            m_users.push_back({"admin", "admin123", UserRole::Admin, "build/assets/levels/level1.json", 100.0f, 0.0f});
            m_users.push_back({"player", "player123", UserRole::Player, "build/assets/levels/level1.json", 100.0f, 0.0f});
            saveUsers();
        }
    }

    std::optional<User> AuthService::authenticate(const std::string& username, const std::string& password)
    {
        for (const auto& user : m_users) {
            if (user.username == username && user.password == password) {
                return User{user.username, user.role, user.currentLevel, user.posX, user.posY};
            }
        }
        return std::nullopt;
    }

    bool AuthService::registerUser(const std::string& username, const std::string& password, UserRole role)
    {
        for (const auto& user : m_users) {
            if (user.username == username) return false; // Already exists
        }
        m_users.push_back({username, password, role, "build/assets/levels/level1.json", 100.0f, 0.0f});
        saveUsers();
        return true;
    }

    void AuthService::updateProgress(const std::string& username, const std::string& level, float x, float y)
    {
        for (auto& user : m_users) {
            if (user.username == username) {
                user.currentLevel = level;
                user.posX = x;
                user.posY = y;
                saveUsers();
                return;
            }
        }
    }

    void AuthService::loadUsers()
    {
        std::ifstream file(m_filePath, std::ios::binary);
        if (!file.is_open()) return;

        std::string encoded((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
        std::string decoded = obfuscate(encoded);

        try {
            json data = json::parse(decoded);
            for (const auto& item : data) {
                m_users.push_back({
                    item["username"],
                    item["password"],
                    static_cast<UserRole>(item["role"]),
                    item["currentLevel"],
                    item["posX"],
                    item["posY"]
                });
            }
        } catch (...) {
            std::cerr << "Failed to parse users.json" << std::endl;
        }
    }

    void AuthService::saveUsers()
    {
        json data = json::array();
        for (const auto& user : m_users) {
            data.push_back({
                {"username", user.username},
                {"password", user.password},
                {"role", static_cast<int>(user.role)},
                {"currentLevel", user.currentLevel},
                {"posX", user.posX},
                {"posY", user.posY}
            });
        }

        std::string jsonStr = data.dump(4); // Pretty print for debug
        std::string encoded = obfuscate(jsonStr);

        // Save obfuscated data
        std::ofstream file(m_filePath, std::ios::binary);
        if (file.is_open()) {
            file << encoded;
        }

        // Save raw data for debugging (DevTools)
        std::ofstream rawFile("build/devtools/raw_users.json");
        if (rawFile.is_open()) {
            rawFile << jsonStr;
        }
    }

    std::string AuthService::obfuscate(const std::string& data)
    {
        std::string output = data;
        const char key = 0x5A; // Simple XOR key
        for (size_t i = 0; i < output.size(); ++i) {
            output[i] ^= key;
        }
        return output;
    }
}
