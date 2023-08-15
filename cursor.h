#pragma once
#include <SDL.h>
#include "main.h"

struct UIManager;


struct Cursor
{
    UIManager* parent = nullptr;
    int x = 0;
    int y = DRAWABLE_Y;
    int tile_x = 0;
    int tile_y = 0;

    void OnKeyPress(SDL_Event e);
    void UpdateToMousePos(int x, int y);
    void Draw(SDL_Renderer* renderer);
};