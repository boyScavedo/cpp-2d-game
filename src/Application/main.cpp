#include <vector>

#include "Engine/InputManager.hpp"
#include "Engine/WindowManager.hpp"
#include "Engine/Renderer.hpp"
#include "Engine/Camera.hpp"

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

    Engine::WindowManager window(Common::WINDOW_TITLE_PREFIX, Common::MINIMUM_SCREEN_WIDTH, Common::MINIMUM_SCREEN_HEIGHT);
    Engine::InputManager inputSystem;
    Engine::Renderer renderer(window.getSDLWindow());
    Engine::Camera camera;

    Gameplay::Player player;

    // Load background textures (start, middle, end tiles for each layer)
    renderer.loadTexture(Common::TextureID::TEX_BACKGROUND_FAR, "assets/sprites/Background_Far_Start.png");
    renderer.loadTexture(Common::TextureID::TEX_BACKGROUND_MID, "assets/sprites/Background_Mid_Start.png");
    renderer.loadTexture(Common::TextureID::TEX_BACKGROUND_NEAR, "assets/sprites/Background_Near_Start.png");

    // For updating the fps counter via fpsCounter()
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

        window.fpsCounter(currentTime, lastFpsTime, fps);

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

        auto [playerX, playerY] = player.getPosition();
        camera.update(playerX);

        renderer.beginFrame();

        frameCommands.clear();

        // Add background layers with parallax scrolling
        // Far background (slowest scroll)
        Common::RenderCommand farBg{0.0f, 0.0f, (float)Common::SCREEN_WIDTH, (float)Common::SCREEN_HEIGHT, Common::TextureID::TEX_BACKGROUND_FAR, 0.2f};
        frameCommands.push_back(farBg);
        // Mid background
        Common::RenderCommand midBg{0.0f, 0.0f, (float)Common::SCREEN_WIDTH, (float)Common::SCREEN_HEIGHT, Common::TextureID::TEX_BACKGROUND_MID, 0.5f};
        frameCommands.push_back(midBg);
        // Near background
        Common::RenderCommand nearBg{0.0f, 0.0f, (float)Common::SCREEN_WIDTH, (float)Common::SCREEN_HEIGHT, Common::TextureID::TEX_BACKGROUND_NEAR, 0.8f};
        frameCommands.push_back(nearBg);

        frameCommands.push_back(player.getRenderCommand());

        // Add camera position indicator (small bar at bottom showing scroll progress)
        float indicatorWidth = (camera.getCameraOffsetX() / Common::MAXIMUM_CAMERA_OFFSET_X) * 200.0f;                        // 200px max width
        frameCommands.push_back({10.0f, Common::SCREEN_HEIGHT - 20.0f, indicatorWidth, 10.0f, Common::TextureID::TEXT_NONE}); // Cyan rectangle

        renderer.drawCommands(frameCommands, camera.getCameraOffsetX());

        renderer.endFrame();
    }

    return 0;
}