# C++ 2D Platformer Game

A 2D sidescrolling roguelike-metroidvania game built using SDL3 and C++20. Features include fluid player movement with physics, collision detection, parallax scrolling backgrounds, JSON-based level loading/saving, in-game level editor, and basic authentication system.

## Features

- **Core Gameplay**: Player movement with gravity, friction, jumping, and collision detection
- **Level System**: JSON-based levels with in-game editor for creating/modifying levels
- **Authentication**: User login/registration system for admin access
- **Admin Tools**: Level editor accessible via CLI dashboard
- **Rendering**: SDL3-based rendering with parallax backgrounds and camera following
- **Data-Driven**: Levels stored as JSON files for easy modding and editing

## Prerequisites

### Windows

- [MinGW (g++) v2.5.0](https://github.com/skeeto/w64devkit/releases) (requires g++ version 15.0.0 or higher)
- [SDL3 v3.4.0](https://github.com/libsdl-org/SDL/releases/download/release-3.4.0/SDL3-devel-3.4.0-mingw.zip)
  - Extract to `C:/SDL3`
- [SDL3_image v3.4.0](https://github.com/libsdl-org/SDL_image/releases/download/release-3.4.0/SDL3_image-devel-3.4.0-mingw.zip)
  - Extract to `C:/SDL3_image`

### Linux/macOS

- GCC 15.0.0+ or Clang with C++20 support
- SDL3 development libraries (install via package manager)
- SDL3_image development libraries

## Build Instructions

1. Clone the repository
2. Ensure prerequisites are installed
3. Run `make` in the project root
4. Run `./build/cpp_2d_game.exe` (Windows) or `./build/cpp_2d_game` (Linux/macOS)

## Usage

1. **Authentication**: Run the game and login/register as admin
2. **Level Editor**: Access admin dashboard to create/edit levels
3. **Gameplay**: Load levels and play with fluid controls (WASD movement, Space jump)

## Project Structure

```
├── src/              # Source files
│   ├── Admin/        # Level editor
│   ├── Application/  # Main entry point
│   ├── Engine/       # Core engine (rendering, input, camera)
│   └── Gameplay/     # Game logic (entities, physics)
├── include/          # Header files
├── assets/           # Game assets (levels, sprites)
├── docs/             # Documentation
├── lib/              # Third-party libraries
└── build/            # Build artifacts
```

## Architecture

- **Engine Layer**: Window management, rendering, input handling, camera
- **Gameplay Layer**: Entity management, physics, collision detection
- **Admin Layer**: Level editor and CLI tools
- **Data-Driven**: JSON level files for content creation

## Contributing

1. Fork the repository
2. Create a feature branch
3. Make changes with proper documentation
4. Test thoroughly
5. Submit a pull request

## License

This project is for educational purposes. See individual files for licensing information.

- Perform `Extract here` with winrar and move the folder to `C:/` and rename it to `SDL3`
- [GNU Make](https://www.gnu.org/software/make/)
  - I installed using [Chocolatey](https://chocolatey.org/) with PowerShell administrator
  ```
  choco install make
  ```
- Visual Studio Code (C/C++ extension) setup
  - save SDL3 folder inside the `C:/` folder
  - create a folder named `.vscode` in the root of the project
  - create a file named `c_cpp_properties.json` in the `.vscode` folder
  - copy the following code into the file
    ```json
    {
      "configurations": [
        {
          "name": "Win32",
          "includePath": [
            "${workspaceFolder}/**",
            "${workspaceFolder}/include",
            "${workspaceFolder}/include/Engine",
            "${workspaceFolder}/include/Gameplay",
            "${workspaceFolder}/include/Core",
            "${workspaceFolder}/include/Utils",
            "C:\\SDL3\\x86_64-w64-mingw32\\include"
          ],
          "defines": ["_DEBUG", "UNICODE", "_UNICODE"],
          "compilerPath": "C:\\w64devkit\\bin/g++.exe",
          "cStandard": "c11",
          "cppStandard": "c++20",
          "intelliSenseMode": "windows-gcc-x64"
        }
      ],
      "version": 4
    }
    ```

# Build

- Open the project in Visual Studio Code
- Open the Terminal
- Run `make`
- Run `make clean` to clean the build for the next build

# Project Structure

```
project/
├── src/
│ ├── main.cpp
│ ├── Engine/
│ │ ├── WindowManager.cpp / .h
│ │ ├── Renderer.cpp / .h
│ │ ├── AudioManager.cpp / .h
│ │ └── InputHandler.cpp / .h
│ ├── Gameplay/
│ │ ├── ECS/
│ │ ├── AI/
│ │ ├── Combat/
│ │ ├── Player/
│ │ └── Enemy/
│ └── Utils/
│ ├── Logging.cpp / .h
│ ├── Math.cpp / .h
│ └── Timer.cpp / .h
├── include/
├── assets/
│ ├── sprites/
│ ├── audio/
│ └── levels/
├── build/
├── Makefile
└── README.md
```

# Branches

- main
  - The main branch of the project. This branch is the one that is being used for the actual gameplay.
- dev
  - The dev branch is the branch that is used for development. It is a copy of the main branch, but with the code that is being worked on.
- feature/branch-name
  - Branches that are used for developing new features or bug fixes.

# Contribution

- Clone the project
- Create a branch for your feature/bug fix
  `git checkout -b feature/branch-name`
- Make your changes
- Commit your changes
- Push your changes
- Create a pull request
