#pragma once

#include <string>
#include <vector>
#include <optional>

namespace Engine
{
    /**
     * @enum UserRole
     * @brief Defines the roles available for users in the authentication system.
     *
     * Roles determine access levels: Players can play the game, Admins can access editing features.
     * This enum supports future expansion for additional roles like Moderators.
     */
    enum class UserRole
    {
        Player, /**< Standard user role for gameplay; limited to playing and basic features. */
        Admin   /**< Elevated role for administrators; grants access to level editing and management tools. */
    };

    /**
     * @struct User
     * @brief Represents a user account with authentication and progress data.
     *
     * This struct holds user information retrieved after successful authentication.
     * It includes role-based permissions and current game state for seamless session resumption.
     * Position data allows saving and loading player progress across levels.
     */
    struct User
    {
        std::string username;     /**< Unique username for identification and display. */
        UserRole role;            /**< User's role determining available features and permissions. */
        std::string currentLevel; /**< Path or name of the level the user was last playing. */
        float posX = 0.0f;        /**< Last known X position in the game world for progress saving. */
        float posY = 0.0f;        /**< Last known Y position in the game world for progress saving. */
    };

    /**
     * @class AuthService
     * @brief Manages user authentication, registration, and progress persistence.
     *
     * The AuthService provides a simple file-based authentication system for the game,
     * allowing multiple users to save their progress securely. It handles user registration,
     * login verification, and progress updates, integrating with the game's save system.
     * Passwords are obfuscated (not encrypted) for basic security; in production, stronger
     * encryption should be implemented. This service decouples user management from gameplay,
     * enabling features like leaderboards or cloud saves in the future.
     */
    class AuthService
    {
    public:
        /**
         * @brief Constructs an AuthService and loads existing user data from file.
         *
         * Initializes the service by attempting to load user data from the JSON file.
         * If the file doesn't exist, starts with an empty user database.
         */
        AuthService();

        /**
         * @brief Authenticates a user with username and password.
         *
         * Verifies the provided credentials against stored user data.
         * Returns user information on success, or empty optional on failure.
         *
         * @param username The username to authenticate.
         * @param password The password for authentication.
         * @return std::optional<User> User data if authentication succeeds, empty otherwise.
         */
        std::optional<User> authenticate(const std::string &username, const std::string &password);

        /**
         * @brief Registers a new user with the given credentials and role.
         *
         * Creates a new user account if the username doesn't already exist.
         * Saves the updated user database to file.
         *
         * @param username The desired username (must be unique).
         * @param password The password for the new account.
         * @param role The role to assign to the new user (default: Player).
         * @return true if registration succeeds, false if username already exists.
         */
        bool registerUser(const std::string &username, const std::string &password, UserRole role = UserRole::Player);

        /**
         * @brief Updates the progress data for an existing user.
         *
         * Saves the current level and position for the specified user.
         * This is called when the player advances or saves progress.
         *
         * @param username The username whose progress to update.
         * @param level The current level path/name.
         * @param x The X position in the level.
         * @param y The Y position in the level.
         */
        void updateProgress(const std::string &username, const std::string &level, float x, float y);

    private:
        /**
         * @struct UserData
         * @brief Internal representation of user data including sensitive information.
         *
         * Stores complete user information, including obfuscated password.
         * This struct is not exposed publicly to maintain security boundaries.
         */
        struct UserData
        {
            std::string username;     /**< Unique username identifier. */
            std::string password;     /**< Obfuscated password string. */
            UserRole role;            /**< User's assigned role. */
            std::string currentLevel; /**< Last played level. */
            float posX;               /**< Saved X position. */
            float posY;               /**< Saved Y position. */
        };

        std::vector<UserData> m_users;                                  /**< In-memory user database. */
        const std::string m_filePath = "build/assets/users/users.json"; /**< Path to user data file. */

        /**
         * @brief Loads user data from the JSON file into memory.
         *
         * Attempts to read and parse the user database file.
         * If the file is missing or corrupted, initializes an empty database.
         */
        void loadUsers();

        /**
         * @brief Saves the current user database to the JSON file.
         *
         * Serializes the user data and writes it to disk.
         * Called after registration or progress updates.
         */
        void saveUsers();

        /**
         * @brief Applies basic obfuscation to sensitive data.
         *
         * Performs a simple transformation on the input string for basic security.
         * Note: This is not strong encryption; use proper cryptography for production.
         *
         * @param data The string to obfuscate.
         * @return std::string The obfuscated string.
         */
        std::string obfuscate(const std::string &data);
    };
}
