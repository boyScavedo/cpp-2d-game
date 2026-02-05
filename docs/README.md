# C++ 2D Game Engine (SDL3)

A bespoke 2D Game Engine built from scratch using C++20 and SDL3. This project demonstrates modern game development patterns, transitioning from OOP to a Data-Driven Entity Component System (ECS).

## Features
- **Core Engine**: SDL3-delimited windowing, input, and rendering.
- **Physics**: Custom AABB physics with gravity, friction, and acceleration.
- **Parallax Scrolling**: Layered background support.
- **Data-Driven Design** (In Progress): Moving to JSON-based level loading.
- **Architecture**: Separated into `Engine`, `Gameplay`, and `Common` modules.

## Roadmap (Academic MVP)
- [ ] **ECS Refactor**: Migrating hardcoded objects to a concise Entity-Component-System.
- [ ] **Level Loading**: Implementing `nlohmann/json` for level definitions.
- [ ] **Tools Integration**: Support for external level editors.

## Prerequisites
- **Compiler**: C++20 compatible (GCC 15.0.0+ / MinGW w64 2.5.0)
- **Library**: SDL3 v3.4.0 (Extracted to `C:/SDL3`)
- **Build System**: GNU Make

## Build Instructions
1. Open terminal in project root.
2. Run `make`.
3. Run `make clean` (optional).
4. Executable is generated in `build/`.

## Project Structure
```text
project/
├── src/            # Source code (Engine, Gameplay, Application)
├── include/        # Header files
├── assets/         # Sprites, Audio, Levels (JSON)
├── docs/           # Documentation & UML
└── Makefile        # Build configuration
```

## Contribution
1. Fork & Clone.
2. Create branch `feature/your-feature`.
3. Commit & Push.
4. Open PR.
