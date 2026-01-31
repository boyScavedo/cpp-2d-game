# Project Documentation for C++ 2D Game

## Introduction

This document serves as a comprehensive guide to the C++ 2D Game project, a sidescrolling roguelike-metroidvania game built using SDL3. The project is structured into several namespaces and modules to organize code logically: `Common` for shared constants and types, `Engine` for core engine components, `Gameplay` for game-specific logic, and `Utils` for utility functions. This documentation explains each module, class, function, variable, and namespace in detail, akin to chapters in a book, providing explanations on their purpose, usage, and interactions.

The game features a player character that can move, jump, and interact in a 2D world with parallax scrolling backgrounds. The engine handles window management, rendering, input, and camera following. The main loop in `main.cpp` orchestrates these components to run the game.

### Project Structure

- `include/`: Header files for all classes and definitions.
- `src/`: Source files implementing the classes.
- `docs/`: Documentation files, including this one and UML diagrams.
- `build/`: Build artifacts (not documented here).

## Chapter 1: Common Namespace

The `Common` namespace contains shared constants and types used across the entire project. These are defined in `include/Common/Constants.hpp` and `include/Common/Types.hpp`.

### Constants

Constants are defined as `inline constexpr` for compile-time evaluation and are grouped by category.

#### Window Settings

- `SCREEN_WIDTH`: The default screen width in pixels (1280). Used for viewport dimensions.
- `SCREEN_HEIGHT`: The default screen height in pixels (720). Used for viewport dimensions.
- `MINIMUM_SCREEN_WIDTH`: The minimum allowed screen width (854). Ensures the game runs on smaller displays.
- `MINIMUM_SCREEN_HEIGHT`: The minimum allowed screen height (480). Ensures the game runs on smaller displays.
- `WINDOW_TITLE_PREFIX`: A string prefix for the window title ("2D Roguelike-Metroidvania v0.0.1 | FPS: "). Appended with FPS for display.

#### Camera Settings

- `VIEWPORT_WIDTH`: The width of the camera viewport, matching `SCREEN_WIDTH` (1280). Defines the visible area.
- `WORLD_WIDTH`: The total width of the game world (2560). Allows for sidescrolling beyond the viewport.
- `MINIMUM_CAMERA_OFFSET_X`: The minimum camera offset (0.0f). Prevents scrolling left of the world origin.
- `MAXIMUM_CAMERA_OFFSET_X`: The maximum camera offset (WORLD_WIDTH - VIEWPORT_WIDTH). Prevents scrolling beyond the world.

#### Player Settings

- `PLAYER_WIDTH`: The width of the player sprite (50.0f). Used for collision and rendering.
- `PLAYER_HEIGHT`: The height of the player sprite (50.0f). Used for collision and rendering.

#### Physics & Gameplay Logic

- `TARGET_FPS`: The target frames per second (60.0f). Used for time-stepping.
- `TIME_STEP`: The fixed time step per frame (1.0f / TARGET_FPS). Ensures consistent physics.
- `GRAVITY`: The gravitational acceleration (980.0f pixels/s²). Applied to vertical velocity.
- `TERMINAL_VELOCITY`: The maximum downward velocity (5399.8f). Prevents infinite falling speed.
- `PLAYER_ACCELERATION`: The horizontal acceleration (5000.0f pixels/s²). How fast the player speeds up.
- `FRICTION`: The friction coefficient (10000.0f). Higher values make movement less slippery.
- `JUMP_FORCE`: The upward force applied on jump (-500.0f). Negative for upward direction.
- `PLAYER_MAX_SPEED`: The maximum horizontal speed (500.0f pixels/s). Caps velocity.

#### Texture & Sprite IDs

- `TextureID`: An enum class for texture identifiers.
  - `TEX_PLAYER`: ID for player texture (0).
  - `TEX_WALL`: ID for wall texture (1).
  - `TEX_FLOOR`: ID for floor texture (2).
  - `TEX_ENEMY`: ID for enemy texture (3).
  - `TEX_BACKGROUND_FAR`: ID for far background (4).
  - `TEX_BACKGROUND_MID`: ID for mid background (5).
  - `TEX_BACKGROUND_NEAR`: ID for near background (6).
  - `TEX_COUNT`: Total texture count.
  - `TEXT_NONE`: No texture (-1), for solid colors.

#### Tile/Grid Settings

- `TILE_SIZE`: The size of each tile in pixels (32). Used for grid-based elements.

### Types

Types are structs defined in `include/Common/Types.hpp`.

#### InputState

A struct representing input from the engine to gameplay.

- `up`, `down`, `left`, `right`: Booleans for directional movement (default false).
- `jump`: Boolean for jump action (default false).
- `attack`: Boolean for attack action (default false).
- `toggleFullScreen`: Boolean for fullscreen toggle (default false).
- `quit`: Boolean for quit signal (default false).

Usage: Updated by `InputManager` and passed to gameplay components for decision-making.

#### RenderCommand

A struct representing a rendering instruction from gameplay to engine.

- `x`, `y`: Position coordinates (default 0.0f).
- `width`, `height`: Dimensions (default 0.0f).
- `textureID`: The texture to use (default TEXT_NONE).
- `scrollFactor`: Parallax factor (0.0f = fixed, 1.0f = full scroll, default 1.0f).

Usage: Collected in a vector and passed to `Renderer` for drawing.

## Chapter 2: Engine Namespace

The `Engine` namespace contains core engine components for window management, rendering, input, and camera control. Classes are defined in `include/Engine/`.

### Camera Class

Located in `include/Engine/Camera.hpp` and `src/Engine/Camera.cpp`.

#### Members

- Private:
  - `cameraOffsetX`: Current horizontal offset (0.0f).
  - `minCameraOffsetX`: Minimum offset (from constants).
  - `maxCameraOffsetX`: Maximum offset (from constants).

#### Methods

- `float getCameraOffsetX() const`: Returns the current offset.
- `void setCameraOffsetX(float value)`: Sets the offset, clamped to min/max.
- `void update(float playerX)`: Updates offset to follow the player, keeping them centered.

Usage: Instantiate once, call `update` with player X position each frame, pass offset to `Renderer`.

### WindowManager Class

Located in `include/Engine/WindowManager.hpp` and `src/Engine/WindowManager.cpp`.

#### Members

- Private:
  - `m_window`: Unique pointer to SDL_Window with custom deleter.
  - `m_sdlInitialized`: Boolean for SDL init status.

#### Methods

- Constructor: `WindowManager(const std::string &title, int width, int height)`: Initializes SDL and creates window.
- Destructor: Cleans up SDL resources.
- `SDL_Window* getSDLWindow() const`: Returns the SDL window pointer (do not delete).
- `void update(const Common::InputState &input)`: Toggles fullscreen on F11.
- `void fpsCounter(const Uint64 &currentTick, Uint64 &lastTime, Uint64 &fps)`: Updates FPS calculation.

Usage: Create at startup, pass window to `Renderer`, call `update` and `fpsCounter` each frame.

### Renderer Class

Located in `include/Engine/Renderer.hpp` and `src/Engine/Renderer.cpp`.

#### Members

- Private:
  - `m_sdlRenderer`: Pointer to SDL_Renderer.
  - `m_textureCache`: Map of TextureID to SDL_Texture\*.

#### Methods

- Constructor: `Renderer(SDL_Window *window)`: Creates SDL renderer.
- Destructor: Cleans up renderer and textures.
- `void beginFrame()`: Clears the screen.
- `void drawCommands(const std::vector<Common::RenderCommand> &commands, float cameraOffsetX)`: Draws all commands with camera offset.
- `void endFrame()`: Presents the frame.

Usage: Create after window, call `beginFrame`, `drawCommands`, `endFrame` each frame.

### InputManager Class

Located in `include/Engine/InputManager.hpp` and `src/Engine/InputManagement.cpp`.

#### Members

- Private:
  - `m_state`: Current InputState.

#### Methods

- `Common::InputState update()`: Polls SDL events, updates state (WASD, Space, Mouse, F11, Quit).

Usage: Create at startup, call `update` each frame, pass result to window and player.

## Chapter 3: Gameplay Namespace

The `Gameplay` namespace contains game-specific logic for the player.

### Player Class

Located in `include/Gameplay/Player.hpp` and `src/Gameplay/Player.cpp`.

#### Members

- Private:
  - `movement`: PlayerMovement instance.

#### Methods

- `void update(float deltaTime, const Common::InputState &input)`: Updates position via movement.
- `Common::RenderCommand getRenderCommand() const`: Returns render command for player.
- `std::pair<float, float> getPosition() const`: Returns current position.

Usage: Create at startup, call `update` each frame, use `getPosition` for camera, `getRenderCommand` for rendering.

### PlayerMovement Class

Located in `include/Gameplay/PlayerMovement.hpp` and `src/Gameplay/PlayerMovement.cpp`.

#### Members

- Private:
  - Constants: acceleration, friction, gravity, etc. (from Common).
  - `m_x`, `m_y`: Position.
  - `m_velocityX`, `m_velocityY`: Velocity.
  - `m_canJump`: Jump availability.

#### Methods

- `void update(float deltaTime, const Common::InputState &input)`: Applies physics, input.
- `Common::RenderCommand getRenderCommand() const`: Returns render command (delegates to Player).
- `std::pair<float, float> getPosition() const`: Returns position.

Usage: Managed by Player, handles movement logic.

## Chapter 4: Utils Namespace

The `Utils` namespace contains utility classes.

### FpsCounter Class

Located in `include/Utils/FpsCounter.hpp` and `src/Utils/FpsCounter.cpp`.

#### Members

- Private:
  - `m_lastTime`, `m_frameCount`, `m_fps`: For calculation.

#### Methods

- `Uint64 update()`: Increments count, calculates FPS every second.

Usage: Call each frame to get current FPS.

### Math Namespace

`include/Utils/Math.hpp` appears empty or minimal; no significant content documented.

## Chapter 5: Application Entry Point

The main application is in `src/Application/main.cpp`.

### Overview

- Initializes window, input, renderer, camera, player.
- Runs a loop: calculates deltaTime, updates FPS, polls input, updates window/player/camera, renders frame.
- Handles quit and fullscreen toggle.

### Key Variables

- `fps`, `lastFpsTime`, `lastTime`: For timing.
- `running`: Loop condition.
- `frameCommands`: Vector of RenderCommand for the frame.

Usage: Compile and run the executable; the game loop runs until quit.

## How to Use the Project

1. **Setup**: Ensure SDL3 is installed. Use the Makefile to build.
2. **Initialization**: Create instances of WindowManager, InputManager, Renderer, Camera, Player.
3. **Loop**: Each frame, update input, window, player, camera; collect render commands; draw.
4. **Customization**: Modify constants for tuning, add new textures/enemies by extending enums and classes.

## Conclusion

This documentation provides a thorough explanation of the C++ 2D Game project. Each component is detailed for understanding and extension. For code changes, refer to the UML diagrams in `UML.md`.
