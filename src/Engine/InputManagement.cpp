/**
 * @file InputManagement.cpp
 * @brief Implementation of the InputManager class for handling SDL input events.
 */

#include "Engine/InputManager.hpp"

/**
 * @brief Polls system input and updates the stored input state.
 *
 * Updates directional and action flags and records high-level input events:
 * - Sets `quit` when a quit event is received.
 * - Sets `toggleFullScreen` when the F11 key is pressed.
 * - Updates `up`, `down`, `left`, `right` from the W/S/A/D keys.
 * - Updates `jump` from the Space key.
 * - Updates `attack` from the left mouse button.
 *
 * @return Common::InputState The updated input state reflecting the current values of
 * `up`, `down`, `left`, `right`, `jump`, `attack`, `toggleFullScreen`, and `quit`.
 */
Common::InputState Engine::InputManager::update()
{
    m_state.toggleFullScreen = false;
    SDL_Event event;

    while (SDL_PollEvent(&event))
    {
        if (event.type == SDL_EVENT_QUIT)
            m_state.quit = true;
        if (event.type == SDL_EVENT_KEY_DOWN)
        {
            if (event.key.scancode == SDL_SCANCODE_F11)
            {
                m_state.toggleFullScreen = true;
            }
        }
    }
    const bool *keys = SDL_GetKeyboardState(nullptr);
    m_state.up = keys[SDL_SCANCODE_W];
    m_state.down = keys[SDL_SCANCODE_S];
    m_state.left = keys[SDL_SCANCODE_A];
    m_state.right = keys[SDL_SCANCODE_D];
    m_state.jump = keys[SDL_SCANCODE_SPACE];
    
    // Arrow Keys
    m_state.arrowUp = keys[SDL_SCANCODE_UP];
    m_state.arrowDown = keys[SDL_SCANCODE_DOWN];
    m_state.arrowLeft = keys[SDL_SCANCODE_LEFT];
    m_state.arrowRight = keys[SDL_SCANCODE_RIGHT];
    
    // Editor Keys mapping
    m_state.c = keys[SDL_SCANCODE_C];
    m_state.backspace = keys[SDL_SCANCODE_BACKSPACE];
    m_state.enter = keys[SDL_SCANCODE_RETURN];
    m_state.shift = keys[SDL_SCANCODE_LSHIFT] || keys[SDL_SCANCODE_RSHIFT];
    m_state.ctrl = keys[SDL_SCANCODE_LCTRL] || keys[SDL_SCANCODE_RCTRL];
    m_state.pause = keys[SDL_SCANCODE_P] || keys[SDL_SCANCODE_ESCAPE];
 
    float mx, my;
    SDL_MouseButtonFlags mouseButtons = SDL_GetMouseState(&mx, &my);
    m_state.mouseX = static_cast<int>(mx);
    m_state.mouseY = static_cast<int>(my);
    m_state.mouseLeftDown = (mouseButtons & SDL_BUTTON_MASK(SDL_BUTTON_LEFT)) != 0;
    m_state.attack = m_state.mouseLeftDown;

    return m_state;
}

Common::InputState Engine::InputManager::update(SDL_Renderer* renderer)
{
    // First, do all the regular input polling
    update();

    // Then convert mouse coordinates from window space to logical renderer space
    if (renderer) {
        float mx, my;
        SDL_GetMouseState(&mx, &my);
        
        float logicalX, logicalY;
        if (SDL_RenderCoordinatesFromWindow(renderer, mx, my, &logicalX, &logicalY)) {
            m_state.mouseX = static_cast<int>(logicalX);
            m_state.mouseY = static_cast<int>(logicalY);
        }
    }

    return m_state;
}
