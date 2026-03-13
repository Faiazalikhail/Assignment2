#include "GameScene.h"


// ======================================================
// TEXT CREATION HELPER
// Creates SDL texture from text string
// ======================================================
SDL_Texture* createText(SDL_Renderer* renderer, TTF_Font* font, const std::string& text)
{
    renderer = ren;

    tableTexture = TextureManager::LoadTexture("../images/TABLE.png", ren);
    cardTexture = TextureManager::LoadTexture("../images/CARDS.png", ren);
    buttonTexture = TextureManager::LoadTexture("../images/BUTTONS.png", ren);
    chipTexture = TextureManager::LoadTexture("../images/CHIPS.png", ren);

    // layout anchors
    dealerY = 160;
    playerY = 420;
    cardStartX = 350;

    hitButton.init(buttonTexture, 2, 360, 650);
    standButton.init(buttonTexture, 3, 560, 650);

    hitButton.setCallback([this]()
        {
            game.playerHit();
        });

    standButton.setCallback([this]()
        {
            game.dealerTurn();
            game.settle();
        });

    game.startRound();
}



// ======================================================
// UPDATE
// Calculates bet values and regenerates UI text
// ======================================================
void GameScene::update()
{
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

    int x = cardStartX;

    for (auto& c : game.dealer.hand.getCards())
    {
        SDL_Rect src;

        src.w = 96;
        src.h = 140;
        src.x = (c.rank - 1) * 96;
        src.y = c.suit * 140;

    SDL_Rect dealerValueRect;

    SDL_QueryTexture(dealerValue, NULL, NULL,
        &dealerValueRect.w, &dealerValueRect.h);

    dealerValueRect.x = 360;
    dealerValueRect.y = 310;

    SDL_RenderCopy(renderer, dealerValue, NULL, &dealerValueRect);
    SDL_DestroyTexture(dealerValue);

    // ======================================================
    // PLAYER HAND LABEL
    // ======================================================

    SDL_Texture* playerLabel =
        createText(renderer, font, "Player Hand");

    SDL_Rect playerLabelRect;

    SDL_QueryTexture(playerLabel, NULL, NULL,
        &playerLabelRect.w, &playerLabelRect.h);

    playerLabelRect.x = 375;
    playerLabelRect.y = 390;

    SDL_RenderCopy(renderer, playerLabel, NULL, &playerLabelRect);
    SDL_DestroyTexture(playerLabel);
    SDL_DestroyTexture(dealerLabel);

    // ======================================================
    // PLAYER HAND(S)
    // ======================================================

    int playerY = 420;

    if (game.state != GameState::Betting && !game.player.hands.empty())
    {
        if (!game.player.isSplit)
        {
            int px = cardStartX;
            for (auto& c : game.player.hands[0].getCards())
            {
                drawCard(c.rank, c.suit, px, playerY);
                px += spacing;
            }
        }
        else
        {
            // Hand 1 Render
            int px1 = cardStartX - 200;
            for (auto& c : game.player.hands[0].getCards())
            {
                drawCard(c.rank, c.suit, px1, playerY);
                px1 += spacing;
            }

            SDL_Texture* hand1Label = createText(renderer, font, "Hand 1");
            SDL_Rect hand1Rect;
            SDL_QueryTexture(hand1Label, NULL, NULL, &hand1Rect.w, &hand1Rect.h);
            hand1Rect.x = cardStartX - 200;
            hand1Rect.y = 600;
            SDL_RenderCopy(renderer, hand1Label, NULL, &hand1Rect);
            SDL_DestroyTexture(hand1Label);

            // Hand 2 Render
            int px2 = cardStartX + 200;
            for (auto& c : game.player.hands[1].getCards())
            {
                drawCard(c.rank, c.suit, px2, playerY);
                px2 += spacing;
            }

        x += 110;
    }

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


    hitButton.render(renderer);
    standButton.render(renderer);
}