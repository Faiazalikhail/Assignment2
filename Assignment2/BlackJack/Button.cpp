#include "Button.h"

Button::Button()
{
    hover = false;
    pressed = false;

    baseW = 133;
    baseH = 64;
}

void Button::init(SDL_Texture* tex, int index, int x, int y)
{
    texture = tex;

    srcRect.x = index * 133;
    srcRect.y = 0;
    srcRect.w = 133;
    srcRect.h = 64;

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

    if (hover)
    {
        draw.w += 8;
        draw.h += 4;
        draw.x -= 4;
        draw.y -= 2;
    }

    SDL_RenderCopy(renderer, texture, &srcRect, &draw);
}