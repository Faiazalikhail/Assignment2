#include "Game.h"
#include "InputManager.h"
#include <SDL_image.h>
#include <iostream>

Game::Game()
{
    window = nullptr;
    renderer = nullptr;
    isRunning = false;
    inGame = false;
}

bool Game::init()
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        std::cout << "SDL Init Failed\n";
        return false;
    }

    if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG))
    {
        std::cout << "IMG Init Failed\n";
        return false;
    }

    window = SDL_CreateWindow(
        "Blackjack",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        1024,
        768,
        0
    );

    if (!window)
    {
        std::cout << "Window creation failed\n";
        return false;
    }

    renderer = SDL_CreateRenderer(
        window,
        -1,
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
    );

    if (!renderer)
    {
        std::cout << "Renderer creation failed\n";
        return false;
    }

    // Initialize scenes
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

    int mouseX = InputManager::getInstance()->getMouseX();
    int mouseY = InputManager::getInstance()->getMouseY();
    bool click = InputManager::getInstance()->isMouseClicked();

    if (!inGame)
    {
        menu.handleEvents(mouseX, mouseY, click);

        if (menu.playPressed())
            inGame = true;
    }
    else
    {
        game.handleEvents(mouseX, mouseY, click);
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
        game.render();

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