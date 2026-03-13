#include "GameScene.h"

void GameScene::init(SDL_Renderer* ren)
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

void GameScene::update()
{
}

void GameScene::render(SDL_Renderer* renderer)
{
    SDL_RenderCopy(renderer, tableTexture, NULL, NULL);

    // ---------------------
    // Dealer cards
    // ---------------------

    int x = cardStartX;

    for (auto& c : game.dealer.hand.getCards())
    {
        SDL_Rect src;

        src.w = 96;
        src.h = 140;
        src.x = (c.rank - 1) * 96;
        src.y = c.suit * 140;

        SDL_Rect dest = { x, dealerY, 96, 140 };

        SDL_RenderCopy(renderer, cardTexture, &src, &dest);

        x += 110;
    }

    // ---------------------
    // Player cards
    // ---------------------

    x = cardStartX;

    for (auto& c : game.player.hand.getCards())
    {
        SDL_Rect src;

        src.w = 96;
        src.h = 140;
        src.x = (c.rank - 1) * 96;
        src.y = c.suit * 140;

        SDL_Rect dest = { x, playerY, 96, 140 };

        SDL_RenderCopy(renderer, cardTexture, &src, &dest);

        x += 110;
    }

    // ---------------------
    // Buttons
    // ---------------------

    hitButton.render(renderer);
    standButton.render(renderer);
}