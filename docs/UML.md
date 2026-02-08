# UML Diagrams for C++ 2D Platformer Game

This document contains UML diagrams generated using Mermaid for the current codebase.

## Class Diagram

```mermaid
classDiagram
    namespace Engine {
        class Camera
        class WindowManager
        class Renderer
        class InputManager
        class LevelLoader
    }
    namespace Gameplay {
        class Entity
        class EntityManager
        class Player
        class StaticObject
    }
    namespace Admin {
        class LevelEditor
    }
    namespace Application {
        class AuthService
        class CLIHandler
    }

    class Camera {
        -float cameraOffsetX
        -float minCameraOffsetX
        -float maxCameraOffsetX
        +float getCameraOffsetX()
        +void setCameraOffsetX(float)
        +float getMinCameraOffsetX()
        +float getMaxCameraOffsetX()
        +void update(float)
    }

    class WindowManager {
        -std::unique_ptr<SDL_Window, SDLDeleter> m_window
        -bool m_sdlInitialized
        +WindowManager(std::string, int, int)
        +~WindowManager()
        +SDL_Window* getSDLWindow()
        +void update(Common::InputState)
        +void fpsCounter(Uint64, Uint64&, Uint64&)
    }

    class Renderer {
        -SDL_Renderer* m_sdlRenderer
        -std::unordered_map<Common::TextureID, SDL_Texture*> m_textureCache
        +Renderer(SDL_Window*)
        +~Renderer()
        +void beginFrame()
        +void drawCommands(std::vector<Common::RenderCommand>, float)
        +void endFrame()
    }

    class InputManager {
        -Common::InputState m_state
        +Common::InputState update()
    }

    class LevelLoader {
        +static bool loadLevel(Gameplay::EntityManager&, const std::string&)
    }

    class Entity {
        <<abstract>>
        +Transform transform
        +Sprite sprite
        +Collider* collider
        +virtual void update(float)
        +virtual std::vector<Common::RenderCommand> getRenderCommands()
    }

    class EntityManager {
        -std::vector<std::shared_ptr<Entity>> m_entities
        -LevelConfig levelConfig
        +void addEntity(std::shared_ptr<Entity>)
        +void removeEntity(std::shared_ptr<Entity>)
        +const std::vector<std::shared_ptr<Entity>>& getEntities()
        +std::string getEntityManagerJSON()
        +bool loadFromJSON(const std::string&)
    }

    class Player {
        +void update(float, Common::InputState)
        +std::vector<Common::RenderCommand> getRenderCommands()
    }

    class StaticObject {
        +void update(float)
        +std::vector<Common::RenderCommand> getRenderCommands()
    }

    class LevelEditor {
        -Gameplay::EntityManager& m_entityManager
        -Engine::Camera& m_camera
        -std::shared_ptr<Entity> m_selectedEntity
        -bool m_active
        +LevelEditor(EntityManager&, Camera&)
        +void update(float, Common::InputState)
        +void render(std::vector<Common::RenderCommand>&)
        +bool isActive()
        +void setActive(bool)
        +bool hasPendingSubmission()
        +std::optional<Suggestion> getPendingSubmission()
    }

    class AuthService {
        +std::optional<Engine::User> authenticate(const std::string&, const std::string&)
        +bool registerUser(const std::string&, const std::string&)
    }

    class CLIHandler {
        +static std::optional<Engine::User> runAuthFlow(AuthService&)
        +static AdminDashboardResult runAdminDashboard(const Engine::User&)
    }

    WindowManager --> Renderer : provides SDL_Window
    EntityManager --> Entity : manages
    Player --|> Entity : inherits
    StaticObject --|> Entity : inherits
    LevelEditor --> EntityManager : edits
    LevelEditor --> Camera : uses
    CLIHandler --> AuthService : uses
    LevelLoader --> EntityManager : loads into
    Renderer --> Entity : draws
    Camera --> Entity : follows
```

## Sequence Diagram

```mermaid
sequenceDiagram
    participant Main
    participant AuthService
    participant CLIHandler
    participant WindowManager
    participant InputManager
    participant Renderer
    participant Camera
    participant EntityManager
    participant LevelEditor

    Main->>AuthService: create
    Main->>CLIHandler: runAuthFlow(authService)
    CLIHandler-->>Main: User (optional)

    alt User authenticated
        Main->>CLIHandler: runAdminDashboard(user)
        CLIHandler-->>Main: AdminDashboardResult

        Main->>WindowManager: create
        Main->>InputManager: create
        Main->>Renderer: create(window)
        Main->>Camera: create
        Main->>EntityManager: create

        alt Create new level
            Main->>EntityManager: initialize empty level
        else Load existing level
            Main->>LevelLoader: loadLevel(entityManager, path)
        end

        alt Enter editor mode
            Main->>LevelEditor: create(entityManager, camera)
            Main->>LevelEditor: setActive(true)
        end

        loop Game Loop
            Main->>InputManager: update()
            InputManager-->>Main: InputState

            alt Editor active
                Main->>LevelEditor: update(deltaTime, input)
                Main->>LevelEditor: render(commands)
            else Game mode
                Main->>EntityManager: update(deltaTime, input)
                Main->>EntityManager: getRenderCommands(commands)
            end

            Main->>Camera: update(playerX)
            Main->>Renderer: beginFrame()
            Main->>Renderer: drawCommands(commands, cameraOffset)
            Main->>Renderer: endFrame()
        end

        alt Level submission pending
            Main->>LevelEditor: getPendingSubmission()
            LevelEditor-->>Main: Suggestion
            Main->>CLIHandler: prompt for level name
            Main->>EntityManager: getEntityManagerJSON()
            Main->>Main: save to build/assets/levels/
            Main->>Main: save to assets/levels/
        end
    end
```
