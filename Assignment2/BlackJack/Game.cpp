#include "Game.h"
#include <SDL_image.h>
#include "InputManager.h"

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
    InputManager::getInstance()->update();

    if (InputManager::getInstance()->isQuit())
        isRunning = false;

    if (!inGame)
    {
        menu.handleEvents(
            InputManager::getInstance()->getMouseX(),
            InputManager::getInstance()->getMouseY(),
            InputManager::getInstance()->isMouseClicked()
        );

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
    menu.clean();
    game.clean();

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    IMG_Quit();
    SDL_Quit();
}

bool Game::running()
{
    return isRunning;
}