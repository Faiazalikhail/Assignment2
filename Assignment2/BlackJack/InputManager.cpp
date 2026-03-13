#include "InputManager.h"

InputManager* InputManager::instance = nullptr;

InputManager::InputManager()
{
    quit = false;
    mouseX = 0;
    mouseY = 0;
    click = false;
}

InputManager* InputManager::getInstance()
{
    if (!instance)
        instance = new InputManager();

    return instance;
}

void InputManager::update()
{
    click = false;

    while (SDL_PollEvent(&event))
    {
        if (event.type == SDL_QUIT)
            quit = true;

        if (event.type == SDL_MOUSEMOTION)
        {
            mouseX = event.motion.x;
            mouseY = event.motion.y;
        }

        if (event.type == SDL_MOUSEBUTTONDOWN)
            click = true;
    }
}

bool InputManager::isQuit()
{
    return quit;
}

int InputManager::getMouseX() const
{
    return mouseX;
}

int InputManager::getMouseY() const
{
    return mouseY;
}

bool InputManager::isMouseClicked() const
{
    return click;
}