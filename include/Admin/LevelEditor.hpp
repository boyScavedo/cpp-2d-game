#pragma once

#include "Engine/Camera.hpp"
#include "Gameplay/EntityManager.hpp"
#include "Common/Types.hpp"
#include <memory>

namespace Admin
{
    /**
     * @class LevelEditor
     * @brief In-game level editing system for creating and modifying game levels.
     *
     * LevelEditor provides a runtime level creation and editing interface accessible to admin users.
     * It allows placing, moving, and deleting entities in real-time, with immediate visual feedback.
     * The editor integrates seamlessly with the gameplay systems, using the same EntityManager and Camera
     * for consistent behavior. Edited levels can be saved as JSON files for distribution. This tool
     * enables content creation without external editors, supporting rapid prototyping and user-generated content.
     * The editor operates in a modal fashion, toggling between play mode and edit mode.
     */
    class LevelEditor
    {
    public:
        /**
         * @brief Constructs a LevelEditor with references to core game systems.
         *
         * Initializes the editor with access to the entity collection and camera for manipulation.
         * The editor starts inactive and must be explicitly enabled.
         *
         * @param entityManager Reference to the game's entity management system.
         * @param camera Reference to the game's camera for coordinate transformations.
         */
        LevelEditor(Gameplay::EntityManager &entityManager, Engine::Camera &camera);

        /**
         * @brief Updates the editor state based on input and time.
         *
         * Processes editor-specific input for entity manipulation, selection, and commands.
         * Handles mouse-based entity selection and movement when active.
         *
         * @param deltaTime Time elapsed since last update in seconds.
         * @param input Current input state for editor controls.
         */
        void update(float deltaTime, const Common::InputState &input);

        /**
         * @brief Renders editor UI elements and visual feedback.
         *
         * Draws selection indicators, entity bounds, and editor interface elements
         * on top of the game world when the editor is active.
         *
         * @param commands Vector to append editor render commands to.
         */
        void render(std::vector<Common::RenderCommand> &commands);

        /**
         * @brief Checks if the level editor is currently active.
         * @return True if the editor is enabled and accepting input.
         */
        bool isActive() const { return m_active; }

        /**
         * @brief Enables or disables the level editor mode.
         * @param active True to activate editor, false to return to play mode.
         */
        void setActive(bool active) { m_active = active; }

        /**
         * @struct Suggestion
         * @brief Contains data for a completed level submission.
         *
         * Encapsulates the information needed to save and potentially connect a newly created level.
         * Includes naming and connection details for level integration.
         */
        struct Suggestion
        {
            std::string levelName;      /**< Name of the created level for file naming. */
            std::string connectionPath; /**< Path to connect this level to existing levels. */
            std::string leadPath;       /**< Path for level progression or navigation. */
        };

        /**
         * @brief Checks if there is a pending level submission.
         * @return True if a submission is pending, false otherwise.
         */
        bool hasPendingSubmission() const { return m_pendingSubmission.has_value(); }

        /**
         * @brief Retrieves and clears any pending level submission.
         *
         * Returns the submission data if the user has completed editing and requested to save.
         * Calling this method consumes the submission (returns it once, then empty on subsequent calls).
         *
         * @return Optional Suggestion containing level submission data, or empty if none pending.
         */
        std::optional<Suggestion> getPendingSubmission()
        {
            auto s = m_pendingSubmission;
            m_pendingSubmission = std::nullopt;
            return s;
        }

    private:
        Gameplay::EntityManager &m_entityManager;           /**< Reference to entity management system. */
        Engine::Camera &m_camera;                           /**< Reference to camera for world coordinates. */
        std::shared_ptr<Gameplay::Entity> m_selectedEntity; /**< Currently selected entity for editing. */
        bool m_active = false;                              /**< Whether editor mode is enabled. */

        std::optional<Suggestion> m_pendingSubmission; /**< Pending level save data. */

        /**
         * @brief Handles entity selection and manipulation via mouse input.
         *
         * Processes mouse clicks for entity selection and drag operations for repositioning.
         * Provides visual feedback for selected entities.
         *
         * @param deltaTime Time elapsed for smooth movement.
         * @param input Current input state for mouse interaction.
         */
        void handleObjectManipulation(float deltaTime, const Common::InputState &input);

        /**
         * @brief Creates a new default entity at the mouse position.
         *
         * Instantiates a basic StaticObject entity with default properties,
         * typically triggered by editor hotkeys.
         */
        void createDefaultObject();

        /**
         * @brief Removes the currently selected entity from the level.
         *
         * Deletes the selected entity from the EntityManager, typically triggered by delete key.
         */
        void deleteSelectedObject();
    };
}
