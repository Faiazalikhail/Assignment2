#pragma once

#include <SDL.h>
#include "TextureManager.h"
#include "Button.h"
#include "BlackJackGame.h"

class GameScene
{
public:

    void init(SDL_Renderer* renderer);

    void update();

    void render(SDL_Renderer* renderer);

    void clean();

private:

    SDL_Renderer* renderer;

    SDL_Texture* tableTexture;
    SDL_Texture* cardTexture;
    SDL_Texture* buttonTexture;
    SDL_Texture* chipTexture;

    BlackJackGame game;

    Button hitButton;
    Button standButton;

    Button chip10Button;
    Button chip50Button;
    Button chip100Button;
    Button clearButton;
    Button dealButton;

    // layout anchors
    int dealerY;
    int playerY;
    int cardStartX;
};