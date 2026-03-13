#pragma once
#include <SDL.h>

class InputManager
{
public:

    static InputManager* getInstance();

    void update();

    bool isQuit();

private:

    InputManager();

    static InputManager* instance;

    SDL_Event event;

    bool quit;
};