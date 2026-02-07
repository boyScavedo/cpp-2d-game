#include "Engine/Camera.hpp"
#include "Admin/LevelEditor.hpp"
#include "Gameplay/Entity.hpp"
#include <iostream>
#include <fstream>

namespace Admin
{
    LevelEditor::LevelEditor(Gameplay::EntityManager& entityManager, Engine::Camera& camera)
        : m_entityManager(entityManager), m_camera(camera), m_active(false) {}

    void LevelEditor::update(float deltaTime, const Common::InputState& input)
    {
        if (!m_active) return;

        handleObjectManipulation(deltaTime, input);

        // Enter triggers submission flag
        static bool enterWasPressed = false;
        if (input.enter && !enterWasPressed) {
             m_pendingSubmission = {"new_level", "", ""}; // Signal main.cpp to take over for CLI input
        }
        enterWasPressed = input.enter;
    }

    void LevelEditor::render(std::vector<Common::RenderCommand>& commands)
    {
        if (!m_active) return;

        // Render gizmo for selected entity
        if (m_selectedEntity) {
            commands.push_back({
                .x = m_selectedEntity->transform.x,
                .y = m_selectedEntity->transform.y,
                .width = m_selectedEntity->transform.width,
                .height = m_selectedEntity->transform.height,
                .textureID = Common::TextureID::TEX_NONE, 
                .scrollFactor = 1.0f,
                .colorR = 150, .colorG = 0, .colorB = 0, .colorA = 255
            });
        }
    }

    void LevelEditor::handleObjectManipulation(float deltaTime, const Common::InputState& input)
    {
        // Object Creation (C)
        static bool cWasPressed = false;
        if (input.c && !cWasPressed) {
            createDefaultObject();
        }
        cWasPressed = input.c;
 
        // Object Deletion (Backspace)
        static bool backspaceWasPressed = false;
        if (input.backspace && !backspaceWasPressed) {
            deleteSelectedObject();
        }
        backspaceWasPressed = input.backspace;
 
        float moveSpeed = 400.0f * deltaTime; 
        float scaleSpeed = 200.0f * deltaTime;
 
        // CAMERA PANNING (Arrows)
        float newCameraX = m_camera.getCameraOffsetX();
        if (input.arrowRight) newCameraX += moveSpeed * 2;
        if (input.arrowLeft) newCameraX -= moveSpeed * 2;
        
        float minCam = m_entityManager.levelConfig.isLeftWallClamped ? m_camera.getMinCameraOffsetX() : -2000.0f;
        float maxCam = m_entityManager.levelConfig.isRightWallClamped ? m_camera.getMaxCameraOffsetX() : m_entityManager.levelConfig.levelWidth + 2000.0f;
        
        m_camera.setCameraOffsetX(std::max(minCam, std::min(maxCam, newCameraX)));
 
        // SELECTION CYCLING (Ctrl + A/D)
        static bool aWasPressed = false;
        static bool dWasPressed = false;
        if (input.ctrl) {
            auto& entities = m_entityManager.getEntities();
            if (!entities.empty()) {
                int index = -1;
                for (int i = 0; i < (int)entities.size(); ++i) {
                    if (entities[i] == m_selectedEntity) {
                        index = i;
                        break;
                    }
                }
 
                if (input.left && !aWasPressed) {
                    index = (index <= 0) ? (int)entities.size() - 1 : index - 1;
                    m_selectedEntity = entities[index];
                }
                if (input.right && !dWasPressed) {
                    index = (index == -1 || index >= (int)entities.size() - 1) ? 0 : index + 1;
                    m_selectedEntity = entities[index];
                }
            }
        }
        aWasPressed = input.left;
        dWasPressed = input.right;

        if (input.ctrl) return; // Prevent movement when cycling

        if (!m_selectedEntity) {
            // WASD Panning when no object selected
            float newCameraX = m_camera.getCameraOffsetX();
            if (input.right) newCameraX += moveSpeed * 2;
            if (input.left) newCameraX -= moveSpeed * 2;
            
            float minCam = m_entityManager.levelConfig.isLeftWallClamped ? m_camera.getMinCameraOffsetX() : -2000.0f;
            float maxCam = m_entityManager.levelConfig.isRightWallClamped ? m_camera.getMaxCameraOffsetX() : m_entityManager.levelConfig.levelWidth + 2000.0f;
            
            m_camera.setCameraOffsetX(std::max(minCam, std::min(maxCam, newCameraX)));
            return;
        }
 
        // Movement & Resizing (WASD)
        if (input.shift) {
            // GROWING
            if (input.right) m_selectedEntity->transform.width += scaleSpeed;
            if (input.left) m_selectedEntity->transform.width = std::max(10.0f, m_selectedEntity->transform.width - scaleSpeed);
            if (input.down) m_selectedEntity->transform.height += scaleSpeed;
            if (input.up) m_selectedEntity->transform.height = std::max(10.0f, m_selectedEntity->transform.height - scaleSpeed);
        } else {
            // MOVING
            if (input.right) m_selectedEntity->transform.x += moveSpeed;
            if (input.left) m_selectedEntity->transform.x -= moveSpeed;
            if (input.down) m_selectedEntity->transform.y += moveSpeed;
            if (input.up) m_selectedEntity->transform.y -= moveSpeed;
        }

        // --- CLAMPING ---
        float minObjX = m_entityManager.levelConfig.isLeftWallClamped ? 0.0f : -2000.0f;
        float maxObjX = m_entityManager.levelConfig.isRightWallClamped ? (m_entityManager.levelConfig.levelWidth - m_selectedEntity->transform.width) : (m_entityManager.levelConfig.levelWidth + 2000.0f);
        float maxObjY = Common::SCREEN_HEIGHT - m_selectedEntity->transform.height;
        
        m_selectedEntity->transform.x = std::max(minObjX, std::min(maxObjX, m_selectedEntity->transform.x));
        m_selectedEntity->transform.y = std::max(0.0f, std::min(maxObjY, m_selectedEntity->transform.y));
 
        // Sync collider with transform
        if (m_selectedEntity->collider) {
            m_selectedEntity->collider->width = m_selectedEntity->transform.width;
            m_selectedEntity->collider->height = m_selectedEntity->transform.height;
        }
    }

    void LevelEditor::createDefaultObject()
    {
        auto obj = std::make_shared<Gameplay::StaticObject>();
        obj->transform = {400, 300, 100, 50, 5};
        obj->sprite = {Common::TextureID::TEX_PLAYER, 5}; // Temporary
        obj->collider = {100, 50, 0, 0, false, true};
        m_entityManager.addEntity(obj);
        m_selectedEntity = obj;
    }

    void LevelEditor::deleteSelectedObject()
    {
        if (m_selectedEntity && !std::dynamic_pointer_cast<Gameplay::Player>(m_selectedEntity)) {
            m_entityManager.removeEntity(m_selectedEntity);
            m_selectedEntity = nullptr;
        }
    }
}
