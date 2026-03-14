#include "Game.h"

int main(int argc, char* argv[])
{
    (void)argc;
    (void)argv;
    Game game;

    if (!game.init("Blackjack", 1024, 768))
        return -1;

    while (game.running())
    {
        game.handleEvents();
        game.update();
        game.render();
    }


    game.clean();

    return 0;
}