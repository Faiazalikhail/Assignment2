#pragma once

#include <SDL.h>
#include "Button.h"

class MenuScene
{
public:

    void init(SDL_Renderer* renderer);

    void handleEvents(int mouseX, int mouseY, bool click);

    void render(SDL_Renderer* renderer);

    bool playPressed();

private:

    SDL_Texture* bgTexture;
    SDL_Texture* logoTexture;
    SDL_Texture* buttonTexture;

    SDL_Rect logoRect;

    Button playButton;
    Button quitButton;

    bool startGame;
};