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

<<<<<<< HEAD

    // ========================================
    // BACKGROUND TEXTURES
    // ========================================

    SDL_Texture* bgTexture = nullptr;
    SDL_Texture* tableTexture = nullptr;
    SDL_Texture* panelTexture = nullptr;


    // ========================================
    // UI TEXTURES
    // ========================================

    SDL_Texture* chipTexture = nullptr;
    SDL_Texture* arrowTexture = nullptr;
    SDL_Texture* buttonTexture = nullptr;


    // ========================================
    // CARD SYSTEM
    // ========================================

    SDL_Texture* cardTexture = nullptr;

    const int CARD_W = 81;
    const int CARD_H = 164;

    void drawCard(int rank, int suit, int x, int y);
    void drawCardBack(int x, int y);


    // ========================================
    // CHIP PANEL
    // ========================================

    SDL_Rect leftArrow[5];
    SDL_Rect rightArrow[5];

    int chipValues[5] = { 5,25,50,100,500 };

    int chipCount[5];


    // ========================================
    // BET SYSTEM
    // ========================================

    int wallet;
    int totalBet;


    // ========================================
    // TEXT SYSTEM
    // ========================================

    TTF_Font* font;

    SDL_Texture* chipCountText[5];
    SDL_Texture* walletText;
    SDL_Texture* betText;


    // ========================================
    // BUTTONS
    // ========================================

    Button playButton;
    Button quitButton;
=======
    BlackJackGame game;
>>>>>>> parent of ec86d49 (FIXed the UI the cards the chip the arrow only logic reamaing)

    Button hitButton;
    Button standButton;
    Button doubleButton;
    Button splitButton;

    Button chip1Button;
    Button chip5Button;
    Button chip10Button;
    Button chip25Button;
    Button chip50Button;

<<<<<<< HEAD
    BlackJackGame game;

    int currentBet = 0;
=======
    Button betButton;
    Button playButton;
    Button quitButton;
    Button clearButton;
    Button dealButton;
>>>>>>> parent of ec86d49 (FIXed the UI the cards the chip the arrow only logic reamaing)

    // layout anchors
    int dealerY;
    int playerY;
    int cardStartX;
};