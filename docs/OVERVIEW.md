# Project Overview: C++ 2D Platformer Game

## Current Status

This project is a 2D sidescrolling roguelike-metroidvania game built using SDL3 and its related libraries. The game features core engine components including window management, rendering, input handling, camera following, authentication, and an in-game level editor. The player character has fluid movement mechanics implemented, including horizontal movement, jumping, gravity simulation, and collision detection. Background parallax scrolling is functional, and the game loop runs at a target 60 FPS.

Key features implemented:

- **Level System**: JSON-based level loading and saving. Levels can be created and edited in-game using the level editor. New levels are saved to both build/assets/levels/ and assets/levels/ directories for development and runtime use.
- **Movement System**: Complete fluid movement with physics (gravity, friction, acceleration), collision detection, and world boundary clamping.
- **Authentication and Saving**: Basic authentication system with user login/registration. Level data is persisted as JSON files.
- **Admin Features**: In-game level editor for creating and modifying levels, accessible via admin dashboard.
- **UI System**: Basic menu system and progress indicators.

The project structure includes organized namespaces (`Common`, `Engine`, `Gameplay`, `Admin`, `Utils`), with header and source files separated. The architecture follows OOP principles with data-driven design elements (JSON level files). Documentation includes docstrings, UML diagrams, and comprehensive project documentation.

## Progress Toward MVP

Last Updated: 2024-12-19

The MVP is defined as achieving a functional level system with editor, fluid movement system with collisions, and basic authentication with level saving.

Based on the current codebase:

- Movement system is 100% complete (physics, input, collisions fully integrated).
- Level system is 90% complete (loading/saving implemented, editor functional, but advanced features like level transitions pending).
- Authentication and saving is 80% complete (auth implemented, level saving works, but no encrypted player profiles yet).

Overall progress: 90%

Progress Bar: ████████████████████████░ (90%)

The project is nearing MVP completion with core gameplay and editing features functional.
