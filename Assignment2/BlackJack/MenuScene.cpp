#include "MenuScene.h"
#include "TextureManager.h"
#include <iostream>

void MenuScene::init(SDL_Renderer* renderer)
{
    bgTexture = TextureManager::LoadTexture("../images/BG.png", renderer);
    logoTexture = TextureManager::LoadTexture("../images/LOGO.png", renderer);
    buttonTexture = TextureManager::LoadTexture("../images/BUTTONS.png", renderer);

    logoRect.w = 700;
    logoRect.h = 230;
    logoRect.x = (1024 - logoRect.w) / 2;
    logoRect.y = 120;

    playButton.init(buttonTexture, 0, 445, 420);
    quitButton.init(buttonTexture, 1, 445, 500);

    startGame = false;

    playButton.setCallback([this]() {
        std::cout << "START GAME\n";
        startGame = true;
        });

    quitButton.setCallback([]() {
        exit(0);
        });
}

void MenuScene::handleEvents(int mx, int my, bool click)
{
    playButton.update(mx, my, click);
    quitButton.update(mx, my, click);
}

void MenuScene::render(SDL_Renderer* renderer)
{
    SDL_RenderCopy(renderer, bgTexture, NULL, NULL);
    SDL_RenderCopy(renderer, logoTexture, NULL, &logoRect);

    playButton.render(renderer);
    quitButton.render(renderer);
}

bool MenuScene::playPressed()
{
    return startGame;
}