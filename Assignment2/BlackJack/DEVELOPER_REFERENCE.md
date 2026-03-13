# C++ SDL2 Blackjack Engine - Comprehensive Developer Reference

This document serves as an exhaustive reference for future AI agents and human developers working on the C++ SDL2 Blackjack engine. It details the architecture, design patterns, functional rules, asset management, data types, classes, specific coordinate values, and future roadmap.

## 1-10: Architecture & Core Loop
1. **Engine Paradigm:** The game uses a simplistic, strict Model-View-Controller (MVC) and state machine architecture separated into Engine, GameLogic, Scenes, and UI layers.
2. **Main Loop:** `main.cpp` initializes the `Game` class and runs a non-blocking `while (game.running())` loop executing `handleEvents()`, `update()`, and `render()`.
3. **No External Frameworks:** The engine strictly forbids introducing new frameworks or heavy abstractions. SDL2 and SDL2_image are the only permitted external libraries.
4. **State Machine (`GameState`):** Core gameplay is driven by `enum class GameState { Betting, PlayerTurn, DealerTurn, GameOver };` found in `BlackJackGame.h`.
5. **Scene Management:** The `Game` class statically holds `MenuScene menu` and `GameScene game`. Transitions are handled via a simple `bool inGame` flag rather than a dynamic scene stack.
6. **Input Routing:** `InputManager` acts as a Singleton (`getInstance()`). Scenes poll `mouseX`, `mouseY`, and `isMouseClicked()` from it rather than handling raw SDL events.
7. **Render Flow:** `SDL_RenderClear` -> `scene.render()` -> `SDL_RenderPresent`. All scenes must adhere to this flow without calling `Present` themselves.
8. **Memory Management:** RAII is used where possible (e.g., `std::vector`), but `SDL_Texture` pointers require explicit `SDL_DestroyTexture` calls in the `clean()` methods of each scene.
9. **Event Polling:** `InputManager::update()` polls `SDL_PollEvent`. Mouse clicks register as `true` for a single frame per `SDL_MOUSEBUTTONDOWN`.
10. **Hardcoded Resolution:** The window is hardcoded to 1024x768 (`main.cpp`). UI coordinates are absolute; there is no dynamic scaling system.

## 11-30: GameLogic Layer & Classes
11. **`Card` Struct:** Holds `int suit` (0-3) and `int rank` (1-13). Contains a `value()` method where face cards return 10 and Aces return 11.
12. **`Deck` Class:** Holds a `std::vector<Card>`. Initializes with 52 cards (4 suits, 13 ranks) and uses `std::mt19937` for shuffling.
13. **`Deck::deal()`:** Returns the next card and increments `index`. Automatically shuffles if `index >= cards.size()`.
14. **`Hand` Class:** Holds a `std::vector<Card>` representing a participant's current cards.
15. **`Hand::getValue()`:** Calculates the optimal blackjack hand value. Iterates cards, sums values, and gracefully downgrades Aces (11 to 1) if `total > 21`.
16. **`Hand::bust()`:** Helper returning `true` if `getValue() > 21`.
17. **`Player` Class:** Holds a `Hand`, `int credits` (defaults to 1000), and `int bet`.
18. **`Player::placeBet(amount)`:** Deducts `amount` from `credits` and adds to `bet` if `amount <= credits`.
19. **`Player::win()`:** Adds `bet * 2` to credits (1:1 payout).
20. **`Player::push()`:** Returns the `bet` to `credits` (tie).
21. **`Dealer` Class:** Holds a `Hand`.
22. **`Dealer::shouldHit()`:** Strictly returns `hand.getValue() < 17`. The dealer must stand on soft 17 (currently doesn't differentiate soft/hard 17).
23. **`BlackJackGame` Class:** The central controller holding `Deck`, `Player`, `Dealer`, and `GameState`.
24. **`BlackJackGame::startRound()`:** Triggers the initial deal (2 cards to player, 2 to dealer) and transitions from `Betting` to `PlayerTurn`.
25. **`BlackJackGame::playerHit()`:** Deals 1 card to the player. Triggers `settle()` if bust, or `dealerTurn()` if exactly 21.
26. **`BlackJackGame::dealerTurn()`:** State transitions to `DealerTurn`. A `while` loop continuously deals to the dealer until `shouldHit()` is false.
27. **`BlackJackGame::settle()`:** Evaluates the win/loss condition. Sets state to `GameOver`.
28. **Natural Blackjack Rule:** In `settle()`, if the player gets 21 on the first 2 cards (and the dealer does not), it pays 3:2 via `player.credits += (int)(player.bet * 2.5);`.
29. **Dealer Win Condition:** If the player busts, the dealer wins immediately (`player.bet = 0; return;`).
30. **Reset Rule:** `resetForNewRound()` must clear hands, reset `bet = 0`, and return state to `Betting`.

## 31-40: UI Framework
31. **`Button` Class:** A lightweight UI element rendering a slice of a spritesheet based on its `index`.
32. **`Button::init()`:** Requires an `SDL_Texture*`, `index` (X-offset multiplier), and `x, y` absolute coordinates.
33. **`Button::update()`:** Checks AABB collision against `mouseX` and `mouseY`. Sets internal `hover` state.
34. **`Button Callback`:** Uses `std::function<void()>` executed when `hover && clicked` is true.
35. **Button Dimensions:** Hardcoded `baseW = 133` and `baseH = 64`.
36. **Hover Effect:** `Button::render()` inflates the destination rect by 8px width and 4px height when `hover` is true to provide visual feedback.
37. **`Chip` Struct:** Simple data container `{ int value; }`, though most chip logic is currently routed through visually repurposed `Button` instances.
38. **Texture Slicing:** The `Button` class assumes the source texture is a horizontal strip where each frame is exactly 133x64 pixels (`srcRect.x = index * 133`).
39. **UI Overlap Risk:** Because input is polled sequentially, if UI elements share AABB space, multiple callbacks can fire simultaneously.
40. **Text Rendering:** Currently, `SDL_ttf` is completely missing. Credit/Bet values are tracked in memory but not rendered on screen.

## 41-55: GameScene Layout Coordinates (Strict Design Spec)
41. **Window Size:** Width: 1024, Height: 768.
42. **Dealer Area Anchor:** `y = 160`.
43. **Player Area Anchor:** `y = 420`.
44. **Card Start Anchor:** `x = 430` (used for both dealer and player).
45. **Card Spacing:** `x += 110` per additional card in the hand.
46. **Action Buttons Row:** `y = 650`.
47. **Hit Button Location:** `x = 260, y = 650`.
48. **Stand Button Location:** `x = 420, y = 650`.
49. **Double Button Location:** `x = 580, y = 650` (Requires logic integration).
50. **Split Button Location:** `x = 740, y = 650` (Requires logic integration).
51. **Betting Panel Column Anchor:** `x = 820`.
52. **Chip Y-Coordinates:** 1: 200, 5: 270, 10: 340, 25: 410, 50: 480.
53. **Betting Button Y-Coordinates:** BET: 560, PLAY (Deal): 630, QUIT: 700.
54. **Card Asset Dimensions:** `w = 96`, `h = 140`.
55. **Visual Hiding:** The dealer's second card is hidden during `PlayerTurn` by rendering a specific sprite offset (assumed row 5, `y = 4 * 140`).

## 56-70: Asset Management & Constants
56. **`TextureManager::LoadTexture()`:** Wraps `IMG_Load` and `SDL_CreateTextureFromSurface`. Always frees the temporary surface.
57. **Background Texture:** `../images/BG.png` (Used in Menu).
58. **Logo Texture:** `../images/LOGO.png` (Used in Menu, centered at 120y).
59. **Table Texture:** `../images/TABLE.png` (Used in GameScene).
60. **Cards Spritesheet:** `../images/CARDS.png` (13 ranks horizontally, 4 suits vertically).
61. **Card Texture Mapping:** `src.x = (rank - 1) * 96`, `src.y = suit * 140`.
62. **Buttons Spritesheet:** `../images/BUTTONS.png` (Horizontal strip, index determines function).
63. **Chips Spritesheet:** `../images/CHIPS.png` (Horizontal strip, index determines denomination).
64. **Chip Denominations Allowed:** 1, 5, 10, 25, 50.
65. **Suit Values:** 0 = Clubs/Spades, 1 = Diamonds/Hearts, 2 = Hearts/Diamonds, 3 = Spades/Clubs (Implementation specific, purely visual mapping).
66. **Rank Values:** 1 = Ace, 11 = Jack, 12 = Queen, 13 = King.
67. **Default Credits:** Player starts with 1000 credits (`Player.h` constructor).
68. **Resource Paths:** Paths are relative to the executable location (e.g., `../images/`). Ensure build artifacts execute in a directory that can resolve this.
69. **No Hardcoded Pointers:** Scene class variables should initialize texture pointers to `nullptr` and nullify them again after `SDL_DestroyTexture`.
70. **Asset Memory Leaks:** If a Scene is initialized twice without `clean()` being called, `LoadTexture` will leak the previous `SDL_Texture*`.

## 71-85: Input, Events, and Callbacks
71. **Callback Lambdas:** UI logic heavily relies on `std::function<void()>`. Capturing `[this]` is standard for Scene methods modifying `GameLogic`.
72. **Input State:** `InputManager` tracks `mouseX`, `mouseY`, `click`, and `quit`.
73. **`SDL_MOUSEMOTION`:** Used to update `mouseX` and `mouseY` continuously.
74. **`SDL_MOUSEBUTTONDOWN`:** Toggles `click = true` for exactly one `InputManager::update()` cycle.
75. **Event Consumption:** Input is *not* consumed upon clicking a button. If two buttons share an AABB, both will trigger.
76. **State Guarding:** Callbacks must manually check `game.state` (e.g., `if (game.state == GameState::Betting)`) to prevent unintended actions.
77. **Clear Button Logic:** Restores `player.credits += player.bet` and sets `player.bet = 0`.
78. **Play Button Logic:** Triggers `startRound()` if `bet > 0`. Acts as a "Next Round" trigger by calling `resetForNewRound()` during `GameOver`.
79. **Quit Button Logic:** Currently mapped to a hard `exit(0);`. Future developers should route this to `isRunning = false` in the `Game` class for a graceful shutdown.
80. **Hit Callback:** Calls `game.playerHit()`.
81. **Stand Callback:** Forcibly ends player turn, calling `game.dealerTurn()` and `game.settle()`.
82. **Double Button (Placeholder):** Renders but lacks game logic. Needs `placeBet(player.bet)`, dealing one card, and forcing stand.
83. **Split Button (Placeholder):** Renders but lacks game logic. Requires architectural changes to `Player` to support multiple `Hand` instances.
84. **Bet Button (Placeholder):** Renders but currently has no function as bets update instantly upon chip click.
85. **Menu Events:** `MenuScene` receives input arguments manually in `handleEvents(mx, my, click)`, unlike `GameScene` which pulls directly from the Singleton.

## 86-100: Roadmap, Known Issues, & Future Tasks
86. **Missing Text Rendering (Priority 1):** The engine lacks `SDL_ttf`. Implementing text is critical to show `Credits`, `Current Bet`, and `Hand Value`.
87. **Double Down Logic (Priority 2):** Update `BlackJackGame` to allow doubling the current bet, receiving exactly one card, and automatically transitioning to `DealerTurn`.
88. **Split Logic (Priority 3):** `Player` class must be refactored from `Hand hand` to `std::vector<Hand> hands`. The main loop must support sequential turns for each hand.
89. **Insurance:** Not currently supported. Requires a new `GameState::Insurance` when the dealer's face-up card is an Ace.
90. **Audio System:** No `SDL_mixer` integration exists. Adding a generic `AudioManager` parallel to `TextureManager` is recommended.
91. **Chip UI Overlap Bug:** If `chip.value > player.credits`, clicking the chip does nothing. There is no visual feedback (e.g., greyed out button or error sound) indicating insufficient funds.
92. **Animation Missing:** Cards snap instantly to their coordinates. Implementing a rudimentary Lerp (Linear Interpolation) based on delta-time would improve feel.
93. **Delta Time:** The `Game::update()` loop is entirely framerate dependent. A `Uint32 lastTime` delta-time calculation should be added to `main.cpp` for physics/animations.
94. **Menu Scene Modernization:** `MenuScene` should use the Singleton `InputManager` instead of taking arguments, aligning its architecture with `GameScene`.
95. **Button Texture Aliasing:** `GameScene.cpp` aliases `playButton` and `dealButton` visually to achieve the layout. This is technical debt and should be cleaned up by unifying the betting/playing state UI logic.
96. **Memory Layout:** The `BlackJackGame` stack-allocates within `GameScene`. If scenes become dynamically loaded via pointers, `BlackJackGame` must be carefully managed to prevent heap leaks.
97. **Soft 17 Rule:** The dealer currently stands on *any* 17. Vegas rules typically require the dealer to hit on a "Soft 17" (A hand containing an Ace valued at 11 totaling 17). `Dealer::shouldHit()` needs updating.
98. **Deck Exhaustion:** If the player plays long enough, `Deck::deal()` shuffles automatically mid-hand. A visual "Shoe Shuffle" notification would be an accurate casino addition.
99. **Resolution Scaling:** Hardcoded UI coordinate mapping (`820`, `650`, etc.) means the game cannot be resized or run fullscreen on different aspect ratios without stretching the viewport.
100. **Compiler Warnings:** Ensure `#pragma once` is respected across platforms. The codebase is heavily reliant on MSVC/GCC forgiving compilation regarding implicit enum conversions.
