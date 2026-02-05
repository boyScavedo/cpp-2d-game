# AI Instruction Guide

## Codebase Overview
This is a C++ 2D Game Engine using SDL3. It currently follows a classic Object-Oriented design pattern but is transitioning to a Data-Driven Entity Component System (ECS).

### Current Architecture (Legacy to be Refactored)
- **Engine**:
    - `Renderer`: Wrapper around `SDL_Renderer`. Handles logical presentation (Letterboxing) and texture caching.
    - `InputManager`: Polls SDL events and returns a POD struct `InputState`.
    - `WindowManager`: specific `SDL_Window` management.
- **Gameplay**:
    - `Player`: Owns `PlayerMovement`.
    - `PlayerMovement`: Hardcoded physics (Gravity, Friction, Acceleration).
- **Common**:
    - `Constants.hpp`: **Crucial**. All magic numbers (Screen size, Gravity, Speed) are here.

### Planned ECS Architecture
- **Registry**: Will replace `Player` object.
- **Components** (`include/Gameplay/ECS/Components.hpp`):
    - `Transform { x, y }`
    - `Velocity { vx, vy }`
    - `Sprite { textureID }`
    - `PlayerControl { }` (Tag)
- **Systems**:
    - `PhysicsSystem`: Will contain the logic currently in `PlayerMovement.cpp`.
    - `RenderSystem`: Will replace `renderer.drawCommands`.

### Coding Standards
- **Naming**: PascalCase for Classes, camelCase for variables/methods, `m_` prefix for private members.
- **Headers**: `#pragma once` used.
- **namespaces**: `Engine`, `Gameplay`, `Common`, `Utils`.

### Critical Rules
- **Do NOT** hardcode "magic numbers" in logic files. Use `Common::Constants`.
- **Do NOT** use `new`/`delete`. Use `std::unique_ptr` or stack allocation.
- **Do NOT** modify `assets` without a clear plan (JSON format).

## User Preferences
- **Best Practices**: User values "Industry Standard" patterns (hence the move to ECS).
- **Documentation**: Keep `docs/` updated.
- **Pact**: No file edits without approval.
