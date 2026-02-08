# AI Instruction Guide

## Codebase Overview

This is a complete C++ 2D Platformer Game using SDL3. It features a functional game with level editor, authentication, and data-driven design using JSON level files.

### Current Architecture

- **Engine Layer**:
  - `Renderer`: Wrapper around `SDL_Renderer`. Handles logical presentation (Letterboxing) and texture caching.
  - `InputManager`: Polls SDL events and returns a POD struct `InputState`.
  - `WindowManager`: Manages `SDL_Window` lifecycle.
  - `Camera`: Handles viewport following and world boundaries.
  - `LevelLoader`: Loads JSON level files into EntityManager.
- **Gameplay Layer**:
  - `Entity`: Base class for game objects (Player, StaticObject).
  - `EntityManager`: Manages all entities and level configuration.
  - Physics with gravity, friction, collision detection.
- **Admin Layer**:
  - `LevelEditor`: In-game level creation and editing system.
- **Application Layer**:
  - `AuthService`: User authentication and registration.
  - `CLIHandler`: Command-line interface for admin dashboard.
- **Common**:
  - `Constants.hpp`: **Crucial**. All magic numbers (Screen size, Gravity, Speed) are here.

### Data-Driven Design

- Levels stored as JSON files in `assets/levels/`
- Entity properties (position, size, sprites, colliders) defined in JSON
- Easy modding and level creation without code changes

### Coding Standards

- **Naming**: PascalCase for Classes, camelCase for variables/methods, `m_` prefix for private members.
- **Headers**: `#pragma once` used.
- **Namespaces**: `Engine`, `Gameplay`, `Admin`, `Common`, `Utils`.
- **Memory**: Use `std::unique_ptr` or stack allocation, no raw `new`/`delete`.

### Critical Rules

- **Do NOT** hardcode "magic numbers" in logic files. Use `Common::Constants`.
- **Do NOT** use `new`/`delete`. Use `std::unique_ptr` or stack allocation.
- **DO** keep `docs/` updated with accurate information.
- **DO** test changes thoroughly before committing.

## User Preferences

- **Best Practices**: Values clean, maintainable code with proper documentation.
- **Testing**: Thorough testing required for all changes.
- **Documentation**: Keep all docs updated to reflect current implementation.
- **Pact**: Changes should be approved and tested before merging.
