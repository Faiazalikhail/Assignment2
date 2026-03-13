#include "InputManager.h"

InputManager* InputManager::instance = nullptr;

InputManager::InputManager()
{
    quit = false;
}

InputManager* InputManager::getInstance()
{
    if (!instance)
        instance = new InputManager();

    return instance;
}

void InputManager::update()
{
    while (SDL_PollEvent(&event))
    {
        if (event.type == SDL_QUIT)
            quit = true;
    }
}

bool InputManager::isQuit()
{
    return quit;
}