#include "GameScene.h"
<<<<<<< HEAD
#include "TextureManager.h"
#include <SDL_ttf.h>
#include <string>
#include <cstdlib>
=======
#include "InputManager.h"
>>>>>>> parent of ec86d49 (FIXed the UI the cards the chip the arrow only logic reamaing)

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
    chip1Button.init(chipTexture, 0, 820, 200);
    chip5Button.init(chipTexture, 1, 820, 270);
    chip10Button.init(chipTexture, 2, 820, 340);
    chip25Button.init(chipTexture, 3, 820, 410);
    chip50Button.init(chipTexture, 4, 820, 480);

    // Betting Panel Buttons (x = 820 implied from layout visual, maybe 820 or centered below chips)
    // we'll place them aligned roughly. Assuming button texture indices based on typical play/quit/bet
    betButton.init(buttonTexture, 4, 820, 560);
    playButton.init(buttonTexture, 5, 820, 630);
    quitButton.init(buttonTexture, 6, 820, 700);

<<<<<<< HEAD
    // The first two sprites (indices 0 and 1) are card backs.
    // Real cards start at index 2.
    src.x = (rank + 2) * CARD_W;
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

    src.x = 0; // Using the first card back (index 0)
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

    // ========================================
    // LOGIC CALLBACKS (DO NOT MODIFY UI VARIABLES)
    // ========================================

    playButton.setCallback([this]() {
        if (game.state == GameState::Betting && totalBet > 0) {
            currentBet = totalBet;
            game.player.bet = currentBet;
            wallet -= totalBet; // Visually update UI wallet
            game.player.credits = wallet; // Sync to logic
            game.startRound();
            for (int i = 0; i < 5; i++) chipCount[i] = 0; // Reset visual chips
        } else if (game.state == GameState::GameOver) {
            game.resetForNewRound();
            for (int i = 0; i < 5; i++) chipCount[i] = 0;
            currentBet = 0;
        }
    });

    quitButton.setCallback([]() {
        exit(0);
    });

    hitButton.setCallback([this]() {
        game.playerHit();
    });

    standButton.setCallback([this]() {
        game.playerStand();
    });

    doubleButton.setCallback([this]() {
        if (wallet >= currentBet) {
            game.playerDouble();
            wallet = game.player.credits;
            currentBet = game.player.bet;
        }
    });

    splitButton.setCallback([this]() {
        if (wallet >= currentBet) {
            game.playerSplit();
            wallet = game.player.credits;
            currentBet = game.player.bet;
        }
    });
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
=======
    // clear and deal button legacy mapped for now or handled elsewhere
    dealButton.init(buttonTexture, 5, 820, 630); // Alias deal to play for now
    clearButton.init(buttonTexture, 6, 820, 700); // Alias clear to quit for now? No, we shouldn't alias.

    hitButton.setCallback([this]()
>>>>>>> parent of ec86d49 (FIXed the UI the cards the chip the arrow only logic reamaing)
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

    chip1Button.setCallback([this](){ if (game.state == GameState::Betting) game.player.placeBet(1); });
    chip5Button.setCallback([this](){ if (game.state == GameState::Betting) game.player.placeBet(5); });
    chip10Button.setCallback([this](){ if (game.state == GameState::Betting) game.player.placeBet(10); });
    chip25Button.setCallback([this](){ if (game.state == GameState::Betting) game.player.placeBet(25); });
    chip50Button.setCallback([this](){ if (game.state == GameState::Betting) game.player.placeBet(50); });

<<<<<<< HEAD
    if (game.state == GameState::PlayerTurn) {
        hitButton.update(mx, my, click);
        standButton.update(mx, my, click);
        doubleButton.update(mx, my, click);
        splitButton.update(mx, my, click);
    }
=======
    betButton.setCallback([this]() { /* Placeholder for bet confirmation if needed */ });
    playButton.setCallback([this]() { dealButton.update(InputManager::getInstance()->getMouseX(), InputManager::getInstance()->getMouseY(), true); });
    quitButton.setCallback([]() { exit(0); });

    clearButton.setCallback([this](){ if (game.state == GameState::Betting) { game.player.credits += game.player.bet; game.player.bet = 0; } });
>>>>>>> parent of ec86d49 (FIXed the UI the cards the chip the arrow only logic reamaing)
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
        chip1Button.update(mouseX, mouseY, click);
        chip5Button.update(mouseX, mouseY, click);
        chip10Button.update(mouseX, mouseY, click);
        chip25Button.update(mouseX, mouseY, click);
        chip50Button.update(mouseX, mouseY, click);
        clearButton.update(mouseX, mouseY, click);
        betButton.update(mouseX, mouseY, click);
        playButton.update(mouseX, mouseY, click);
        quitButton.update(mouseX, mouseY, click);
    } else if (game.state == GameState::GameOver) {
        dealButton.update(mouseX, mouseY, click); // Reused as "Next Round"
        quitButton.update(mouseX, mouseY, click);
    }
<<<<<<< HEAD

    // ========================================
    // ROUND RESULT TEXT
    // ========================================

    if (roundResultText)
        SDL_DestroyTexture(roundResultText);

    if (game.state == GameState::GameOver) {
        roundResult = game.resultMessage;
        wallet = game.player.credits; // update wallet visually from logic engine
    } else {
        roundResult = " ";
    }

    roundResultText =
        createText(renderer, font, roundResult);


    // ========================================
    // CURRENT BET DURING ROUND
    // ========================================

    if (currentBetRoundText)
        SDL_DestroyTexture(currentBetRoundText);

    currentBetTextString = "Current Bet: " + std::to_string(currentBet);
    currentBetRoundText =
        createText(renderer, font, currentBetTextString);
=======
>>>>>>> parent of ec86d49 (FIXed the UI the cards the chip the arrow only logic reamaing)
}

void GameScene::render(SDL_Renderer* renderer)
{
    SDL_RenderCopy(renderer, tableTexture, NULL, NULL);

    // ---------------------
    // Dealer cards
    // ---------------------

    int x = cardStartX;
    bool hideSecondCard = (game.state == GameState::PlayerTurn && game.dealer.hand.getCards().size() >= 2);

<<<<<<< HEAD
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


    int dealerY = 160;
    int dealerStartX = 430;

    if (game.state != GameState::Betting && !game.dealer.hand.getCards().empty())
    {
        bool hideSecondCard = (game.state == GameState::PlayerTurn && game.dealer.hand.getCards().size() >= 2);
        int dx = dealerStartX;
        int index = 0;

        for (auto& c : game.dealer.hand.getCards())
        {
            if (hideSecondCard && index == 1)
            {
                drawCardBack(dx, dealerY);
            }
            else
            {
                drawCard(c.rank, c.suit, dx, dealerY);
            }
            dx += 110;
            index++;
        }
    }


    std::string dVal = "Hand Value: ";
    if (game.state != GameState::Betting && game.state != GameState::PlayerTurn) {
        dVal += std::to_string(game.dealer.hand.getValue());
    } else {
        dVal += "?";
    }

    SDL_Texture* dealerValue =
        createText(renderer, font, dVal);

    SDL_Rect dealerValueRect;

    SDL_QueryTexture(dealerValue, NULL, NULL,
        &dealerValueRect.w, &dealerValueRect.h);

    dealerValueRect.x = 360;
    dealerValueRect.y = 250;

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
    playerLabelRect.y = 310;

    SDL_RenderCopy(renderer, playerLabel, NULL, &playerLabelRect);
    SDL_DestroyTexture(playerLabel);
    SDL_DestroyTexture(dealerLabel);

    // ======================================================
    // PLAYER HAND 1
    // ======================================================

    int playerY = 420;
    int playerStartX = 430;

    if (game.state != GameState::Betting && !game.player.hands.empty())
    {
        if (!game.player.isSplit)
        {
            int px = playerStartX;
            for (auto& c : game.player.hands[0].getCards())
            {
                drawCard(c.rank, c.suit, px, playerY);
                px += 110;
            }
        }
        else
        {
            // Split hands appear side-by-side
            int px1 = playerStartX - 150;
            for (auto& c : game.player.hands[0].getCards())
            {
                drawCard(c.rank, c.suit, px1, playerY);
                px1 += 110;
            }

            int px2 = playerStartX + 150;
            for (auto& c : game.player.hands[1].getCards())
            {
                drawCard(c.rank, c.suit, px2, playerY);
                px2 += 110;
            }
        }
    }

    SDL_Texture* hand1Label =
        createText(renderer, font, "Hand 1");

    SDL_Rect hand1Rect;

    SDL_QueryTexture(hand1Label, NULL, NULL,
        &hand1Rect.w, &hand1Rect.h);

    hand1Rect.x = 265;
    hand1Rect.y = 520;

    SDL_RenderCopy(renderer, hand1Label, NULL, &hand1Rect);
    SDL_DestroyTexture(hand1Label);

    // ---------------------------
    // RIGHT PANEL / HAND 2
    // ---------------------------

    if (game.player.isSplit && game.player.hands.size() > 1)
    {
        SDL_Texture* hand2Label =
            createText(renderer, font, "Hand 2 (if split)");

        SDL_Rect hand2Rect;

        SDL_QueryTexture(hand2Label, NULL, NULL,
            &hand2Rect.w, &hand2Rect.h);

        hand2Rect.x = 500;
        hand2Rect.y = 520;

        SDL_RenderCopy(renderer, hand2Label, NULL, &hand2Rect);
        SDL_DestroyTexture(hand2Label);
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
=======
    int cardIndex = 0;
    for (auto& c : game.dealer.hand.getCards())
>>>>>>> parent of ec86d49 (FIXed the UI the cards the chip the arrow only logic reamaing)
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
        chip1Button.render(renderer);
        chip5Button.render(renderer);
        chip10Button.render(renderer);
        chip25Button.render(renderer);
        chip50Button.render(renderer);
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