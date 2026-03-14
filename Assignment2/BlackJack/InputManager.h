#pragma once
#include <SDL.h>

class InputManager
{
public:

    static InputManager* getInstance();

    void update();

    bool isQuit();

    int getMouseX() const;
    int getMouseY() const;
    bool isMouseClicked() const;

private:

    InputManager();

    static InputManager* instance;

    SDL_Event event;

    bool quit;
    int mouseX;
    int mouseY;
    bool click;
};