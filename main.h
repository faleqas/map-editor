#pragma once
#include <SDL.h>
#include <SDL_TTF.h>


const int WINDOW_WIDTH = 1024;
const int WINDOW_HEIGHT = 640; //not starting from drawable_y
const int TILES_WIDTH = 16; //width of screen by tiles
const int TILES_HEIGHT = 10;
const int TILE_WIDTH = WINDOW_WIDTH / TILES_WIDTH;
const int TILE_HEIGHT = WINDOW_HEIGHT / TILES_HEIGHT;
const int DRAWABLE_Y = TILE_HEIGHT * 2;//start of drawable area on screen

void DrawText(SDL_Renderer* renderer, TTF_Font* font, const char* text, int x, int y, int r, int g, int b);