#include "GameScene.h"
#include "InputManager.h"

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

    // UI layout: Buttons row 1 (actions), row 2 (betting)
    dealButton.init(buttonTexture, 0, 460, 650);
    clearButton.init(buttonTexture, 1, 800, 550); // reused quit button texture roughly

    // We can use small offsets for chips visually, but we need buttons to add bet
    // Repurposing some indices from BUTTONS.png as a placeholder for chip interaction
    chip10Button.init(chipTexture, 0, 800, 200);
    chip50Button.init(chipTexture, 1, 800, 300);
    chip100Button.init(chipTexture, 2, 800, 400);

    hitButton.setCallback([this]()
        {
            game.playerHit();
        });

    standButton.setCallback([this]()
        {
            if (game.state == GameState::PlayerTurn) {
                game.dealerTurn();
                game.settle();
            }
        });

    dealButton.setCallback([this]()
        {
            if (game.state == GameState::Betting && game.player.bet > 0) {
                game.startRound();
            } else if (game.state == GameState::GameOver) {
                game.resetForNewRound();
            }
        });

    chip10Button.setCallback([this](){ if (game.state == GameState::Betting) game.player.placeBet(10); });
    chip50Button.setCallback([this](){ if (game.state == GameState::Betting) game.player.placeBet(50); });
    chip100Button.setCallback([this](){ if (game.state == GameState::Betting) game.player.placeBet(100); });
    clearButton.setCallback([this](){ if (game.state == GameState::Betting) { game.player.credits += game.player.bet; game.player.bet = 0; } });

}

void GameScene::update()
{
    int mouseX = InputManager::getInstance()->getMouseX();
    int mouseY = InputManager::getInstance()->getMouseY();
    bool click = InputManager::getInstance()->isMouseClicked();

    if (game.state == GameState::PlayerTurn) {
        hitButton.update(mouseX, mouseY, click);
        standButton.update(mouseX, mouseY, click);
    } else if (game.state == GameState::Betting) {
        dealButton.update(mouseX, mouseY, click);
        chip10Button.update(mouseX, mouseY, click);
        chip50Button.update(mouseX, mouseY, click);
        chip100Button.update(mouseX, mouseY, click);
        clearButton.update(mouseX, mouseY, click);
    } else if (game.state == GameState::GameOver) {
        dealButton.update(mouseX, mouseY, click); // Reused as "Next Round"
    }
}

void GameScene::render(SDL_Renderer* renderer)
{
    SDL_RenderCopy(renderer, tableTexture, NULL, NULL);

    // ---------------------
    // Dealer cards
    // ---------------------

    int x = cardStartX;
    bool hideSecondCard = (game.state == GameState::PlayerTurn && game.dealer.hand.getCards().size() >= 2);

    int cardIndex = 0;
    for (auto& c : game.dealer.hand.getCards())
    {
        SDL_Rect src;

        if (hideSecondCard && cardIndex == 1) {
            // Render back of card
            src.w = 96;
            src.h = 140;
            src.x = 0; // Assume top left of card texture or a specific offset has back
            src.y = 4 * 140; // Usually row 5 is the back in these spritesheets
        } else {
            src.w = 96;
            src.h = 140;
            src.x = (c.rank - 1) * 96;
            src.y = c.suit * 140;
        }

        SDL_Rect dest = { x, dealerY, 96, 140 };

        SDL_RenderCopy(renderer, cardTexture, &src, &dest);

        x += 110;
        cardIndex++;
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

    if (game.state == GameState::PlayerTurn) {
        hitButton.render(renderer);
        standButton.render(renderer);
    } else if (game.state == GameState::Betting) {
        dealButton.render(renderer);
        chip10Button.render(renderer);
        chip50Button.render(renderer);
        chip100Button.render(renderer);
        clearButton.render(renderer);
    } else if (game.state == GameState::GameOver) {
        dealButton.render(renderer);
    }
}

void GameScene::clean()
{
    SDL_DestroyTexture(tableTexture);
    tableTexture = nullptr;

    SDL_DestroyTexture(cardTexture);
    cardTexture = nullptr;

    SDL_DestroyTexture(buttonTexture);
    buttonTexture = nullptr;

    SDL_DestroyTexture(chipTexture);
    chipTexture = nullptr;
}