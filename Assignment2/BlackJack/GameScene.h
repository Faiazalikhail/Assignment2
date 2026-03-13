#pragma once

#include <string>
#include <SDL.h>
#include <SDL_ttf.h>
#include "Button.h"
#include "BlackJackGame.h"

class GameScene
{
public:

    void init(SDL_Renderer* r);
    void handleEvents(int mx, int my, bool click);
    void update();
    void render();
    void clean();


private:

    // ========================================
    // CORE RENDERER
    // ========================================

    SDL_Renderer* renderer = nullptr;


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

    Button hitButton;
    Button standButton;
    Button doubleButton;
    Button splitButton;

    // Added to hook logic properly without touching old UI vars where possible
    void resetChipCounts();

    // ========================================
    // ROUND INFORMATION
    // ========================================

    BlackJackGame game;

    int currentBet = 0;

    std::string roundResult = "";
    std::string currentBetTextString = "";

    // UI textures
    SDL_Texture* roundResultText = nullptr;
    SDL_Texture* currentBetRoundText = nullptr;
};