#include "tile.h"
#include <SDL_image.h>
#include <stdio.h>


Tile::Tile(SDL_Renderer* renderer, int id, const char* path)
{
    {
        SDL_Surface* surface = IMG_Load(path);
        if (surface)
        {
            _texture = SDL_CreateTextureFromSurface(renderer, surface);
            SDL_FreeSurface(surface);
        }
        else
        {
            printf("WARNING: Failed to load image of tile with id '%d'\n", id);
        }
    }
    this->id = id;
}

Tile::~Tile()
{
    SDL_DestroyTexture(_texture);
}

void Tile::Draw(SDL_Renderer* renderer, int x, int y, bool in_edit_area)
{
    SDL_Rect dest = {x * TILE_WIDTH, y * TILE_WIDTH, TILE_WIDTH, TILE_HEIGHT};
    if (in_edit_area)
    {
        dest.y += DRAWABLE_Y;
    }
    SDL_RenderCopy(renderer, _texture, nullptr, &dest);
}