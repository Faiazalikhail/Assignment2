#pragma once
#include <SDL.h>
#include "MenuScene.h"
#include "GameScene.h"

class Game
{
public:

    Game();
    ~Game();

    bool init(const char* title, int width, int height);

    void handleEvents();
    void update();
    void render();
    void clean();

    bool running();

private:

    SDL_Window* window;
    SDL_Renderer* renderer;

    bool isRunning;

    MenuScene menu;
    GameScene game;

    bool inGame;

    int mouseX;
    int mouseY;
    bool click;
};