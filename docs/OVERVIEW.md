# Project Overview: C++ 2D Platformer Game

## Current Status

This project is a 2D sidescrolling roguelike-metroidvania game built using SDL3 and its related libraries. The game is in early development stages, with core engine components established, including window management, rendering, input handling, and camera following. The player character has basic movement mechanics implemented, including horizontal movement, jumping, and gravity simulation. Background parallax scrolling is functional, and the game loop runs at a target 60 FPS.

However, key MVP features are not yet implemented:

- **Level System**: No level world generation, transitioning between levels, or progress saving between levels exists.
- **Movement System**: While basic fluid movements are present, advanced features like collision detection with environments are partially implemented but not fully integrated.
- **Authentication and Saving**: No basic authentication for multiple players or encrypted data saving for player profiles is in place. Currently, there is no persistence of game data beyond a single session.

The project structure includes organized namespaces (`Common`, `Engine`, `Gameplay`, `Utils`), with header and source files separated. Documentation is being added via docstrings, and a TODO list tracks remaining tasks like completing docstrings and potentially expanding gameplay features.

## Progress Toward MVP

The MVP is defined as achieving a functional level system, fluid movement system, and basic authentication for saving progress across multiple players with encrypted data.

Based on the current codebase:

- Movement system is approximately 70% complete (basic physics and input handling are done, but collision integration is missing).
- Level system is 0% complete (no implementation).
- Authentication and saving is 0% complete (no implementation).

Overall progress: 23%

Progress Bar: ███████░░░░░░░░░░░░░░░░░ (23%)

This score reflects the foundational work on movement, but the absence of levels and saving systems keeps the project far from MVP readiness.
