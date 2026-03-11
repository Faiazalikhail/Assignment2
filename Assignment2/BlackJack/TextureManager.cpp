#include "TextureManager.h"

SDL_Texture* TextureManager::LoadTexture(const char* file, SDL_Renderer* ren)
{
    SDL_Surface* surface = IMG_Load(file);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(ren, surface);
    SDL_FreeSurface(surface);
    return texture;
}