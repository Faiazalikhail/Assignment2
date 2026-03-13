#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <string>

class TextureManager
{
public:

    static SDL_Texture* LoadTexture(const std::string& file, SDL_Renderer* renderer);
};