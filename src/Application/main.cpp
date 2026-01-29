#include "Engine/WindowManager.hpp"

int main(int argc, char *argv[])
{
    // Suppress unused parameter warnings
    (void)argc;
    (void)argv;

    Engine::WindowManager game("Red Window Demo", 800, 600);

    while (game.isRunning())
    {
        game.pollEvents();
        game.render();
    }

    return 0;
}