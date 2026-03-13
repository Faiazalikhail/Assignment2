#include "TextureManager.h"
#include <iostream>

SDL_Texture* TextureManager::LoadTexture(const std::string& file, SDL_Renderer* renderer)
{
    SDL_Surface* surface = IMG_Load(file.c_str());

    if (!surface)
    {
        std::cout << "Failed to load texture: " << file << std::endl;
        return nullptr;
    }

    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);

    SDL_FreeSurface(surface);

    if (!texture)
    {
        std::cout << "Failed to create texture: " << file << std::endl;
    }

    return texture;
}