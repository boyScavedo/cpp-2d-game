#include "Engine/WindowManager.hpp"
#include "Gameplay/Player.hpp"

int main(int argc, char *argv[])
{
    // Suppress unused parameter warnings
    (void)argc;
    (void)argv;

    Engine::WindowManager game("2D Movement Demo");
    Gameplay::Player player(100.0f, 100.0f);

    Uint64 lastTime = SDL_GetTicks();

    SDL_Renderer *renderer = game.getRenderer();

    while (game.isRunning())
    {
        Uint64 currentTime = SDL_GetTicks();
        float deltaTime = (currentTime - lastTime) / 1000.0f;
        lastTime = currentTime;

        game.pollEvents();

        player.update(deltaTime);

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        SDL_SetRenderDrawColor(renderer, 50, 0, 0, 255); // Dark red
        SDL_FRect background = {0, 0, 1280, 720};
        SDL_RenderFillRect(renderer, &background);

        player.draw(renderer);
        SDL_RenderPresent(renderer);
    }

    return 0;
}