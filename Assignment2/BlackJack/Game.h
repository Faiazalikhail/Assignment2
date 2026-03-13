#pragma once

#include <SDL.h>
#include "MenuScene.h"
#include "GameScene.h"

class Game
{
public:

    Game();

    bool init();

    void handleEvents();

    void update();

    void render();

    void clean();

    bool running();

private:

    SDL_Window* window;
    SDL_Renderer* renderer;

    bool isRunning;
    bool inGame;

    MenuScene menu;
    GameScene game;
};