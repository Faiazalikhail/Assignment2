#include "Button.h"

Button::Button()
{
    hover = false;
    pressed = false;

    // default button display size
    baseW = 134;
    baseH = 48;
}

void Button::init(SDL_Texture* tex, int index, int x, int y)
{
    texture = tex;

    // --------------------------------------------------
    // BUTTON SPRITE SETTINGS
    // spritesheet = 938 x 48
    // 7 buttons horizontally
    // --------------------------------------------------

    const int BUTTON_WIDTH = 134;
    const int BUTTON_HEIGHT = 48;

    srcRect.x = index * BUTTON_WIDTH;
    srcRect.y = 0;
    srcRect.w = BUTTON_WIDTH;
    srcRect.h = BUTTON_HEIGHT;

    destRect.x = x;
    destRect.y = y;
    destRect.w = baseW;
    destRect.h = baseH;
}

void Button::setCallback(std::function<void()> func)
{
    callback = func;
}

void Button::update(int mx, int my, bool clicked)
{
    hover =
        mx >= destRect.x &&
        mx <= destRect.x + destRect.w &&
        my >= destRect.y &&
        my <= destRect.y + destRect.h;

    if (hover && clicked && callback)
        callback();
}

void Button::render(SDL_Renderer* renderer)
{
    SDL_Rect draw = destRect;

    // --------------------------------------------------
    // SIMPLE HOVER EFFECT
    // --------------------------------------------------

    if (hover)
    {
        draw.w += 8;
        draw.h += 4;
        draw.x -= 4;
        draw.y -= 2;
    }

    SDL_RenderCopy(renderer, texture, &srcRect, &draw);
}