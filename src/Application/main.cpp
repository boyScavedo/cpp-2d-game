#include <vector>

#include "Engine/InputManager.hpp"
#include "Engine/WindowManager.hpp"
#include "Engine/Renderer.hpp"

#include "Gameplay/Player.hpp"

#include "Common/Constants.hpp"

/**
 * @brief Application entry point that initializes engine subsystems and runs the main game loop.
 *
 * Initializes the window, input manager, renderer, and player; then enters a loop that
 * performs time-stepping, input polling (including quit handling), game update, and
 * frame command collection/rendering until the application exits.
 *
 * @return int Exit code; `0` indicates successful termination.
 */
int main(int argc, char *argv[])
{
    // Suppress unused parameter warnings
    (void)argc;
    (void)argv;

    Engine::WindowManager window(Common::WINDOW_TITLE, Common::MINIMUM_SCREEN_WIDTH, Common::MINIMUM_SCREEN_HEIGHT);
    Engine::InputManager inputSystem;
    Engine::Renderer renderer(window.getSDLWindow());

    Gameplay::Player player(100.0f, 100.0f);
    Uint64 fps = 0;
    Uint64 lastFpsTime = 0;
    Uint64 lastTime = SDL_GetTicks();
    bool running = true;
    while (running)
    {
        std::vector<Common::RenderCommand> frameCommands;
        frameCommands.reserve(16);

        Uint64 currentTime = SDL_GetTicks();
        float deltaTime = (currentTime - lastTime) / 1000.0f;
        lastTime = currentTime;

        window.fpsCounter(&currentTime, &lastFpsTime, &fps);

        if (deltaTime > 0.1f)
        {
            deltaTime = 0.1f;
        }

        Common::InputState currentInput = inputSystem.update();

        if (currentInput.quit)
        {
            running = false;
        }
        window.update(currentInput);

        player.update(deltaTime, currentInput);

        renderer.beginFrame();

        frameCommands.clear();
        frameCommands.push_back(player.getRenderCommand());

        renderer.drawCommands(frameCommands);

        renderer.endFrame();
    }

    return 0;
}