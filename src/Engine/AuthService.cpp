/**
 * @file AuthService.cpp
 * @brief Implementation of the AuthService class for user authentication and progress persistence.
 *
 * This file provides the concrete implementation of user management functionality,
 * including authentication, registration, and progress saving. It uses JSON serialization
 * with basic obfuscation for data persistence, supporting both admin and player roles
 * with different access levels in the game.
 */

#include <iostream>
#include <fstream>
#include "Engine/AuthService.hpp"
#include "nlohmann/json.hpp"

using json = nlohmann::json;

namespace Engine
{
    /**
     * @brief Constructs an AuthService instance and initializes user data.
     *
     * Loads existing user data from the JSON file. If no users exist (first run),
     * creates default admin and player accounts with mock data for development.
     * This ensures the application has at least basic authentication capabilities.
     */
    AuthService::AuthService()
    {
        loadUsers();
        if (m_users.empty())
        {
            // Initial mock data if no file exists
            m_users.push_back({"admin", "admin123", UserRole::Admin, "build/assets/levels/level1.json", 100.0f, 0.0f});
            m_users.push_back({"player", "player123", UserRole::Player, "build/assets/levels/level1.json", 100.0f, 0.0f});
            saveUsers();
        }
    }

    /**
     * @brief Authenticates a user with username and password.
     *
     * Searches through stored users to find a match. If found, returns a User object
     * with the user's details (excluding password for security). Used during login
     * to verify credentials and establish user session.
     *
     * @param username The username to authenticate.
     * @param password The password to verify.
     * @return std::optional<User> containing user data if authentication succeeds, std::nullopt otherwise.
     */
    std::optional<User> AuthService::authenticate(const std::string &username, const std::string &password)
    {
        for (const auto &user : m_users)
        {
            if (user.username == username && user.password == password)
            {
                return User{user.username, user.role, user.currentLevel, user.posX, user.posY};
            }
        }
        return std::nullopt;
    }

    /**
     * @brief Registers a new user account.
     *
     * Creates a new user account if the username doesn't already exist.
     * New users start with default progress (level1.json at position 100,0).
     * Admin role allows access to level editor, while Player role is for gameplay only.
     *
     * @param username The desired username (must be unique).
     * @param password The password for the new account.
     * @param role The user role (Admin or Player).
     * @return true if registration succeeds, false if username already exists.
     */
    bool AuthService::registerUser(const std::string &username, const std::string &password, UserRole role)
    {
        for (const auto &user : m_users)
        {
            if (user.username == username)
                return false; // Already exists
        }
        m_users.push_back({username, password, role, "build/assets/levels/level1.json", 100.0f, 0.0f});
        saveUsers();
        return true;
    }

    /**
     * @brief Updates a user's progress and saves to persistent storage.
     *
     * Modifies the user's current level and position data, then persists the changes.
     * Called during gameplay to save progress between sessions, allowing players
     * to resume from their last position.
     *
     * @param username The username whose progress to update.
     * @param level The current level path.
     * @param x The player's X position in the level.
     * @param y The player's Y position in the level.
     */
    void AuthService::updateProgress(const std::string &username, const std::string &level, float x, float y)
    {
        for (auto &user : m_users)
        {
            if (user.username == username)
            {
                user.currentLevel = level;
                user.posX = x;
                user.posY = y;
                saveUsers();
                return;
            }
        }
    }

    /**
     * @brief Loads user data from the obfuscated JSON file.
     *
     * Reads the users.json file, deobfuscates the content, and parses it into
     * the internal user vector. Handles file I/O errors gracefully and provides
     * basic error logging for debugging. If parsing fails, the user list remains empty.
     */
    void AuthService::loadUsers()
    {
        std::ifstream file(m_filePath, std::ios::binary);
        if (!file.is_open())
            return;

        std::string encoded((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
        std::string decoded = obfuscate(encoded);

        try
        {
            json data = json::parse(decoded);
            for (const auto &item : data)
            {
                m_users.push_back({item["username"],
                                   item["password"],
                                   static_cast<UserRole>(item["role"]),
                                   item["currentLevel"],
                                   item["posX"],
                                   item["posY"]});
            }
        }
        catch (...)
        {
            std::cerr << "Failed to parse users.json" << std::endl;
        }
    }

    /**
     * @brief Saves user data to the obfuscated JSON file.
     *
     * Serializes the user vector to JSON format, obfuscates it for basic security,
     * and writes it to the users.json file. Also saves a raw (unobfuscated) version
     * to devtools for debugging purposes. Ensures data persistence across application runs.
     */
    void AuthService::saveUsers()
    {
        json data = json::array();
        for (const auto &user : m_users)
        {
            data.push_back({{"username", user.username},
                            {"password", user.password},
                            {"role", static_cast<int>(user.role)},
                            {"currentLevel", user.currentLevel},
                            {"posX", user.posX},
                            {"posY", user.posY}});
        }

        std::string jsonStr = data.dump(4); // Pretty print for debug
        std::string encoded = obfuscate(jsonStr);

        // Save obfuscated data
        std::ofstream file(m_filePath, std::ios::binary);
        if (file.is_open())
        {
            file << encoded;
        }

        // Save raw data for debugging (DevTools)
        std::ofstream rawFile("build/devtools/raw_users.json");
        if (rawFile.is_open())
        {
            rawFile << jsonStr;
        }
    }

    /**
     * @brief Applies basic obfuscation to data using XOR encryption.
     *
     * Uses a simple XOR operation with a fixed key for basic data obfuscation.
     * This is not strong encryption but provides minimal protection against casual
     * inspection of user data files. The same function is used for both encoding and decoding.
     *
     * @param data The string data to obfuscate/deobfuscate.
     * @return The obfuscated/deobfuscated string.
     */
    std::string AuthService::obfuscate(const std::string &data)
    {
        std::string output = data;
        const char key = 0x5A; // Simple XOR key
        for (size_t i = 0; i < output.size(); ++i)
        {
            output[i] ^= key;
        }
        return output;
    }
}
