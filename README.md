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

# Dependencies

- SDL3
