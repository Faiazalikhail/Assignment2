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
    cardStartX = 430;

    // Action Buttons (y = 650)
    hitButton.init(buttonTexture, 0, 260, 650);
    standButton.init(buttonTexture, 1, 420, 650);
    doubleButton.init(buttonTexture, 2, 580, 650); // assuming index 2 is double
    splitButton.init(buttonTexture, 3, 740, 650); // assuming index 3 is split

    // Betting Panel Chips (x = 820)
    chip10Button.init(chipTexture, 0, 820, 200);
    chip50Button.init(chipTexture, 1, 820, 270);
    chip100Button.init(chipTexture, 2, 820, 340);
    chip500Button.init(chipTexture, 3, 820, 410);
    chip1000Button.init(chipTexture, 4, 820, 480);

    // Betting Panel Buttons (x = 820 implied from layout visual, maybe 820 or centered below chips)
    // we'll place them aligned roughly. Assuming button texture indices based on typical play/quit/bet
    betButton.init(buttonTexture, 4, 820, 560);
    playButton.init(buttonTexture, 5, 820, 630);
    quitButton.init(buttonTexture, 6, 820, 700);

    // clear and deal button legacy mapped for now or handled elsewhere
    dealButton.init(buttonTexture, 5, 820, 630); // Alias deal to play for now
    clearButton.init(buttonTexture, 6, 820, 700); // Alias clear to quit for now? No, we shouldn't alias.

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
    chip500Button.setCallback([this](){ if (game.state == GameState::Betting) game.player.placeBet(500); });
    chip1000Button.setCallback([this](){ if (game.state == GameState::Betting) game.player.placeBet(1000); });

    betButton.setCallback([this]() { /* Placeholder for bet confirmation if needed */ });
    playButton.setCallback([this]() { dealButton.update(InputManager::getInstance()->getMouseX(), InputManager::getInstance()->getMouseY(), true); });
    quitButton.setCallback([]() { exit(0); });

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
        doubleButton.update(mouseX, mouseY, click);
        splitButton.update(mouseX, mouseY, click);
    } else if (game.state == GameState::Betting) {
        dealButton.update(mouseX, mouseY, click);
        chip10Button.update(mouseX, mouseY, click);
        chip50Button.update(mouseX, mouseY, click);
        chip100Button.update(mouseX, mouseY, click);
        chip500Button.update(mouseX, mouseY, click);
        chip1000Button.update(mouseX, mouseY, click);
        clearButton.update(mouseX, mouseY, click);
        betButton.update(mouseX, mouseY, click);
        playButton.update(mouseX, mouseY, click);
        quitButton.update(mouseX, mouseY, click);
    } else if (game.state == GameState::GameOver) {
        dealButton.update(mouseX, mouseY, click); // Reused as "Next Round"
        quitButton.update(mouseX, mouseY, click);
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
        doubleButton.render(renderer);
        splitButton.render(renderer);
    } else if (game.state == GameState::Betting) {
        // dealButton.render(renderer);  -- Using play button visually
        chip10Button.render(renderer);
        chip50Button.render(renderer);
        chip100Button.render(renderer);
        chip500Button.render(renderer);
        chip1000Button.render(renderer);
        betButton.render(renderer);
        playButton.render(renderer);
        quitButton.render(renderer);
        // clearButton.render(renderer);
    } else if (game.state == GameState::GameOver) {
        playButton.render(renderer); // Using play for deal visually
        quitButton.render(renderer);
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