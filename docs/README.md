# C++ 2D Platformer Game (SDL3)

A complete 2D sidescrolling roguelike-metroidvania game built using C++20 and SDL3. Features fluid player movement, collision detection, JSON-based level system with in-game editor, authentication, and data-driven design.

## Features

- **Core Gameplay**: Player physics with gravity, friction, jumping, and collision detection
- **Level System**: JSON-based level loading/saving with in-game level editor
- **Authentication**: User login/registration system with admin dashboard
- **Admin Tools**: Level editor for creating/modifying game levels
- **Rendering**: SDL3-based rendering with parallax backgrounds and camera following
- **Data-Driven Design**: Levels stored as JSON files for easy modding

## Prerequisites

- **Compiler**: C++20 compatible (GCC 15.0.0+ / MinGW w64 2.5.0)
- **Libraries**:
  - SDL3 v3.4.0 (Extracted to `C:/SDL3`)
  - SDL3_image v3.4.0 (Extracted to `C:/SDL3_image`)
- **Build System**: GNU Make

## Build Instructions

1. Open terminal in project root.
2. Run `make`.
3. Run `make clean` (optional).
4. Executable is generated in `build/`.

## Usage

1. Run the executable
2. Login/register as admin user
3. Access admin dashboard to create/edit levels or start game
4. Use WASD for movement, Space for jump in gameplay
5. Use level editor controls (C to create objects, Backspace to delete, etc.)

## Project Structure

```text
project/
├── src/            # Source code (Engine, Gameplay, Admin, Application)
├── include/        # Header files
├── assets/         # Sprites, Audio, Levels (JSON)
├── docs/           # Documentation & UML
├── lib/            # Third-party libraries (nlohmann/json, googletest)
└── build/          # Build artifacts
```

## Architecture

- **Engine Layer**: Core systems (Window, Renderer, Input, Camera, LevelLoader)
- **Gameplay Layer**: Game logic (Entities, EntityManager, Physics)
- **Admin Layer**: Level editor and CLI tools
- **Data-Driven**: JSON level files for content creation

## Contribution

1. Fork & Clone.
2. Create branch `feature/your-feature`.
3. Commit & Push.
4. Open PR.
