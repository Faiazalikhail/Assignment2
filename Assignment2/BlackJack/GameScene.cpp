#include "GameScene.h"
#include "TextureManager.h"
#include <SDL_ttf.h>
#include <string>


// ======================================================
// TEXT CREATION HELPER
// Creates SDL texture from text string
// ======================================================
SDL_Texture* createText(SDL_Renderer* renderer, TTF_Font* font, const std::string& text)
{
    SDL_Color white = { 255,255,255 };

    SDL_Surface* surface = TTF_RenderText_Blended(font, text.c_str(), white);

    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);

    SDL_FreeSurface(surface);

    return texture;
}

    // ======================================================
    // DRAW CARD FROM SPRITESHEET
    // ======================================================
void GameScene::drawCard(int rank, int suit, int x, int y)
{
    SDL_Rect src;

    src.x = rank * CARD_W;
    src.y = suit * CARD_H;
    src.w = CARD_W;
    src.h = CARD_H;

    SDL_Rect dest;

    dest.x = x;
    dest.y = y;
    dest.w = CARD_W;
    dest.h = CARD_H;

    SDL_RenderCopy(renderer, cardTexture, &src, &dest);
}

void GameScene::drawCardBack(int x, int y)
{
    SDL_Rect src;

    src.x = 0;
    src.y = 0;
    src.w = CARD_W;
    src.h = CARD_H;

    SDL_Rect dest = { x,y,CARD_W,CARD_H };

    SDL_RenderCopy(renderer, cardTexture, &src, &dest);
}

// ======================================================
// INITIALIZATION
// Loads textures, fonts and prepares UI elements
// ======================================================
void GameScene::init(SDL_Renderer* r)
{
    renderer = r;

    // ---------------------------
    // BACKGROUND LAYERS
    // ---------------------------

    bgTexture = TextureManager::LoadTexture("../images/BG.png", renderer);
    tableTexture = TextureManager::LoadTexture("../images/TABLE.png", renderer);
    panelTexture = TextureManager::LoadTexture("../images/CHIPBG.png", renderer);

    // ---------------------------
    // UI SPRITES
    // ---------------------------

    chipTexture = TextureManager::LoadTexture("../images/CHIPS.png", renderer);
    arrowTexture = TextureManager::LoadTexture("../images/ARROWS.png", renderer);
    buttonTexture = TextureManager::LoadTexture("../images/BUTTONS.png", renderer);

    // ---------------------------
    // FONT SYSTEM
    // ---------------------------

    TTF_Init();
    font = TTF_OpenFont("../fonts/ds-digit.ttf", 22);

    // ---------------------------
    // CHIP PANEL LAYOUT
    // ---------------------------

    int rows[5] = { 110,210,310,410,510 };

    for (int i = 0; i < 5; i++)
    {
        leftArrow[i] = { 845, rows[i], 26, 40 };
        rightArrow[i] = { 965, rows[i], 26, 40 };
    }

    // ---------------------------
    // PLAYER ECONOMY
    // ---------------------------

    wallet = 1000;
    totalBet = 0;

    for (int i = 0; i < 5; i++)
    {
        chipCount[i] = 0;
        chipCountText[i] = nullptr;
    }

    walletText = nullptr;
    betText = nullptr;

    // ======================================================
    // PANEL BUTTONS (RIGHT SIDE)
    // ======================================================

    playButton.init(buttonTexture, 4, 845, 620);   // BET
    quitButton.init(buttonTexture, 1, 845, 680);   // QUIT


    // ======================================================
    // TABLE ACTION BUTTONS (BOTTOM CENTER)
    // ======================================================

    hitButton.init(buttonTexture, 2, 120, 660);      // HIT
    standButton.init(buttonTexture, 3, 270, 660);    // STAND
    doubleButton.init(buttonTexture, 5, 420, 660);   // DOUBLE
    splitButton.init(buttonTexture, 6, 570, 660);    // SPLIT

    // ================================
    // CARD SPRITESHEET
    // ================================

    cardTexture = TextureManager::LoadTexture("../images/CARDS.png", renderer);

	// round initialization

    currentBet = 0;

    roundResult = "";
    currentBetTextString = "Current Bet: 0";
}

    


// ======================================================
// INPUT HANDLING
// Handles chip arrows and button interactions
// ======================================================
void GameScene::handleEvents(int mx, int my, bool click)
{
    int rows[5] = { 100,200,300,400,500 };

    // ---------------------------
    // CHIP ARROW CONTROLS
    // ---------------------------

    for (int i = 0; i < 5; i++)
    {
        SDL_Rect L = leftArrow[i];
        SDL_Rect R = rightArrow[i];

        bool hoverL =
            mx >= L.x && mx <= L.x + L.w &&
            my >= L.y && my <= L.y + L.h;

        bool hoverR =
            mx >= R.x && mx <= R.x + R.w &&
            my >= R.y && my <= R.y + R.h;

        if (click)
        {
            if (hoverL && chipCount[i] > 0)
                chipCount[i]--;

            if (hoverR)
            {
                int futureBet = totalBet + chipValues[i];

                if (futureBet <= wallet)
                    chipCount[i]++;
            }
        }
    }

    // ---------------------------
    // BUTTON INPUT
    // ---------------------------

    playButton.update(mx, my, click);
    quitButton.update(mx, my, click);

    hitButton.update(mx, my, click);
    standButton.update(mx, my, click);
    doubleButton.update(mx, my, click);
    splitButton.update(mx, my, click);
}



// ======================================================
// UPDATE
// Calculates bet values and regenerates UI text
// ======================================================
void GameScene::update()
{
    totalBet = 0;

    for (int i = 0; i < 5; i++)
        totalBet += chipCount[i] * chipValues[i];

    if (walletText) SDL_DestroyTexture(walletText);
    if (betText) SDL_DestroyTexture(betText);

    walletText = createText(renderer, font, "Wallet: " + std::to_string(wallet));
    betText = createText(renderer, font, "Current Bet: " + std::to_string(totalBet));

    for (int i = 0; i < 5; i++)
    {
        if (chipCountText[i]) SDL_DestroyTexture(chipCountText[i]);

        chipCountText[i] =
            createText(renderer, font, "x" + std::to_string(chipCount[i]));
    }

    // ========================================
    // ROUND RESULT TEXT
    // ========================================

    if (roundResultText)
        SDL_DestroyTexture(roundResultText);

    roundResultText =
        createText(renderer, font, roundResult);


    // ========================================
    // CURRENT BET DURING ROUND
    // ========================================

    if (currentBetRoundText)
        SDL_DestroyTexture(currentBetRoundText);

    currentBetRoundText =
        createText(renderer, font, currentBetTextString);
}



// ======================================================
// RENDER
// Draws full scene in layered order
// ======================================================
void GameScene::render()
{
    // ---------------------------
    // BACKGROUND
    // ---------------------------

    SDL_Rect bgRect = { 0,0,1024,768 };
    SDL_RenderCopy(renderer, bgTexture, NULL, &bgRect);

    // ---------------------------
    // TABLE
    // ---------------------------

    SDL_Rect tableRect = { 0,0,820,650 };
    SDL_RenderCopy(renderer, tableTexture, NULL, &tableRect);

    // ======================================================
    // DEALER HAND PLACEHOLDER
    // ======================================================

    SDL_Texture* dealerLabel =
        createText(renderer, font, "Dealer Hand");

    SDL_Rect dealerTextRect;

    SDL_QueryTexture(dealerLabel, NULL, NULL,
        &dealerTextRect.w, &dealerTextRect.h);

    dealerTextRect.x = 375;
    dealerTextRect.y = 50;

    SDL_RenderCopy(renderer, dealerLabel, NULL, &dealerTextRect);


    // example placeholder cards
	drawCard(3, 0, 300, 80);      // 3 clubs
    drawCardBack(440, 80);      // hidden dealer card


    SDL_Texture* dealerValue =
        createText(renderer, font, "Hand Value:");

    SDL_Rect dealerValueRect;

    SDL_QueryTexture(dealerValue, NULL, NULL,
        &dealerValueRect.w, &dealerValueRect.h);

    dealerValueRect.x = 360;
    dealerValueRect.y = 250;

    SDL_RenderCopy(renderer, dealerValue, NULL, &dealerValueRect);

    // ======================================================
    // PLAYER HAND LABEL
    // ======================================================

    SDL_Texture* playerLabel =
        createText(renderer, font, "Player Hand");

    SDL_Rect playerLabelRect;

    SDL_QueryTexture(playerLabel, NULL, NULL,
        &playerLabelRect.w, &playerLabelRect.h);

    playerLabelRect.x = 375;
    playerLabelRect.y = 310;

    SDL_RenderCopy(renderer, playerLabel, NULL, &playerLabelRect);

    // ======================================================
    // PLAYER HAND 1
    // ======================================================

    drawCard(8, 0, 230, 340);
    drawCard(8, 0, 270, 350);

    SDL_Texture* hand1Label =
        createText(renderer, font, "Hand 1");

    SDL_Rect hand1Rect;

    SDL_QueryTexture(hand1Label, NULL, NULL,
        &hand1Rect.w, &hand1Rect.h);

    hand1Rect.x = 265;
    hand1Rect.y = 520;

    SDL_RenderCopy(renderer, hand1Label, NULL, &hand1Rect);

    // ---------------------------
    // RIGHT PANEL
    // ---------------------------

    // ======================================================
    // PLAYER HAND 2 PLACEHOLDER
    // ======================================================

    drawCard(8, 0, 480, 340);
    drawCard(8, 0, 520, 350);

    SDL_Texture* hand2Label =
        createText(renderer, font, "Hand 2 (if split)");

    SDL_Rect hand2Rect;

    SDL_QueryTexture(hand2Label, NULL, NULL,
        &hand2Rect.w, &hand2Rect.h);

    hand2Rect.x = 500;
    hand2Rect.y = 520;

    SDL_RenderCopy(renderer, hand2Label, NULL, &hand2Rect);

    SDL_Rect panelRect = { 820,0,204,768 };
    SDL_RenderCopy(renderer, panelTexture, NULL, &panelRect);


    int mouseX, mouseY;
    SDL_GetMouseState(&mouseX, &mouseY);

    int chipW = 323 / 5;
    int chipH = 133 / 2;
    int chipX = 885;

    int rows[5] = { 100,200,300,400,500 };


    // ======================================================
    // CHIP PANEL
    // ======================================================

    for (int i = 0; i < 5; i++)
    {
        int y = rows[i];

        int chipCols[5] = { 1,3,4,0,1 };
        int chipRows[5] = { 0,0,0,1,1 };

        SDL_Rect chipSrc =
        {
            chipCols[i] * chipW,
            chipRows[i] * chipH,
            chipW,
            chipH
        };

        SDL_Rect chipDest =
        {
            chipX,
            y,
            chipW,
            chipH
        };

        SDL_RenderCopy(renderer, chipTexture, &chipSrc, &chipDest);


        // ---------------------------
        // ARROWS
        // ---------------------------

        SDL_Rect srcLeft = { 0,0,26,40 };
        SDL_Rect srcRight = { 26,0,26,40 };

        SDL_Rect destL = leftArrow[i];
        SDL_Rect destR = rightArrow[i];

        if (mouseX >= destL.x && mouseX <= destL.x + destL.w &&
            mouseY >= destL.y && mouseY <= destL.y + destL.h)
        {
            destL.w += 4;
            destL.h += 4;
        }

        if (mouseX >= destR.x && mouseX <= destR.x + destR.w &&
            mouseY >= destR.y && mouseY <= destR.y + destR.h)
        {
            destR.w += 4;
            destR.h += 4;
        }

        SDL_RenderCopy(renderer, arrowTexture, &srcLeft, &destL);
        SDL_RenderCopy(renderer, arrowTexture, &srcRight, &destR);


        // ---------------------------
        // CHIP COUNT TEXT
        // ---------------------------

        SDL_Rect textRect;

        SDL_QueryTexture(chipCountText[i], NULL, NULL, &textRect.w, &textRect.h);

        textRect.x = chipX + (chipW / 2) - (textRect.w / 2);
        textRect.y = y + chipH + 8;

        SDL_RenderCopy(renderer, chipCountText[i], NULL, &textRect);
    }



    // ======================================================
    // PANEL BUTTONS
    // ======================================================

    playButton.render(renderer);
    quitButton.render(renderer);



    // ======================================================
    // TABLE ACTION BUTTONS
    // ======================================================

    hitButton.render(renderer);
    standButton.render(renderer);
    doubleButton.render(renderer);
    splitButton.render(renderer);



    // ======================================================
    // WALLET TEXT
    // ======================================================

    SDL_Rect walletRect;

    SDL_QueryTexture(walletText, NULL, NULL, &walletRect.w, &walletRect.h);

    walletRect.x = 820 + (204 / 2) - (walletRect.w / 2);
    walletRect.y = 30;

    SDL_RenderCopy(renderer, walletText, NULL, &walletRect);



    // ======================================================
    // CURRENT BET TEXT
    // ======================================================

    SDL_Rect betRect;

    SDL_QueryTexture(betText, NULL, NULL, &betRect.w, &betRect.h);

    betRect.x = 820 + (204 / 2) - (betRect.w / 2);
    betRect.y = 65;

    SDL_RenderCopy(renderer, betText, NULL, &betRect);

    // ========================================
    // ROUND RESULT (TOP LEFT)
    // ========================================

    if (roundResultText)
    {
        SDL_Rect resultRect;

        SDL_QueryTexture(
            roundResultText,
            NULL,
            NULL,
            &resultRect.w,
            &resultRect.h
        );

        resultRect.x = 40;
        resultRect.y = 30;

        SDL_RenderCopy(renderer, roundResultText, NULL, &resultRect);
    }

    // ========================================
    // CURRENT BET (TOP RIGHT)
    // ========================================

    if (currentBetRoundText)
    {
        SDL_Rect betRect;

        SDL_QueryTexture(
            currentBetRoundText,
            NULL,
            NULL,
            &betRect.w,
            &betRect.h
        );

        betRect.x = 615;
        betRect.y = 30;

        SDL_RenderCopy(renderer, currentBetRoundText, NULL, &betRect);
    }
}



// ======================================================
// CLEANUP
// Releases all textures and font resources
// ======================================================
void GameScene::clean()
{
    SDL_DestroyTexture(bgTexture);
    SDL_DestroyTexture(tableTexture);
    SDL_DestroyTexture(panelTexture);
    SDL_DestroyTexture(chipTexture);
    SDL_DestroyTexture(arrowTexture);
    SDL_DestroyTexture(buttonTexture);
    if (cardTexture)
    {
        SDL_DestroyTexture(cardTexture);
        cardTexture = nullptr;
    }
    if (roundResultText)
    {
        SDL_DestroyTexture(roundResultText);
        roundResultText = nullptr;
    }

    if (currentBetRoundText)
    {
        SDL_DestroyTexture(currentBetRoundText);
        currentBetRoundText = nullptr;
    }

    for (int i = 0; i < 5; i++)
        SDL_DestroyTexture(chipCountText[i]);

    SDL_DestroyTexture(walletText);
    SDL_DestroyTexture(betText);

    TTF_CloseFont(font);
    TTF_Quit();
}