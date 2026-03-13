#include "Game.h"
#include <SDL_image.h>

Game::Game()
{
    isRunning = false;
    inGame = false;
}

Game::~Game() {}

bool Game::init(const char* title, int width, int height)
{
    SDL_Init(SDL_INIT_VIDEO);
    IMG_Init(IMG_INIT_PNG);

    window = SDL_CreateWindow(
        title,
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        width,
        height,
        SDL_WINDOW_SHOWN
    );

    renderer = SDL_CreateRenderer(
        window,
        -1,
        SDL_RENDERER_ACCELERATED
    );

    menu.init(renderer);
    game.init(renderer);

    isRunning = true;

    return true;
}

void Game::handleEvents()
{
    SDL_Event event;
    click = false;

    while (SDL_PollEvent(&event))
    {
        if (event.type == SDL_QUIT)
            isRunning = false;

        if (event.type == SDL_MOUSEMOTION)
        {
            mouseX = event.motion.x;
            mouseY = event.motion.y;
        }

        if (event.type == SDL_MOUSEBUTTONDOWN)
            click = true;
    }

    if (!inGame)
    {
        menu.handleEvents(mouseX, mouseY, click);

        if (menu.playPressed())
            inGame = true;
    }
}

void Game::update()
{
    if (inGame)
        game.update();
}

void Game::render()
{
    SDL_RenderClear(renderer);

    if (!inGame)
        menu.render(renderer);
    else
        game.render(renderer);

    SDL_RenderPresent(renderer);
}

void Game::clean()
{
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    IMG_Quit();
    SDL_Quit();
}

bool Game::running()
{
    return isRunning;
}