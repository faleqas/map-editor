#pragma once
#include <SDL.h>
#include "main.h"

struct TileManager;



struct Tile
{
    int id = 0;
    SDL_Texture* _texture = nullptr;
    TileManager* manager = nullptr;

    Tile(SDL_Renderer* renderer, int id, const char* path);
    ~Tile();
    //X and Y in tile space
    void Draw(SDL_Renderer* renderer, int x, int y, bool in_edit_area = true);
};