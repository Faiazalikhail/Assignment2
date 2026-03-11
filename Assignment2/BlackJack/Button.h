#pragma once
#include <SDL.h>
#include <functional>

class Button
{
public:

    Button();

    void init(SDL_Texture* texture, int index, int x, int y);

    void update(int mouseX, int mouseY, bool clicked);

    void render(SDL_Renderer* renderer);

    void setCallback(std::function<void()> func);

private:

    SDL_Texture* texture;

    SDL_Rect srcRect;
    SDL_Rect destRect;

    bool hover;
    bool pressed;

    int baseW;
    int baseH;

    std::function<void()> callback;
};