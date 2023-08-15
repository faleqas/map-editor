#pragma once
#include <SDL.h>
#include <SDL_TTF.h>

enum
{
    BUTTON_NONE,
    BUTTON_OPEN,
    BUTTON_SAVE,
    BUTTON_LIST_TILES,
    BUTTON_CHANGE_INPUT,
    BUTTON_MAP_WIDTH,
    BUTTON_MAP_HEIGHT
};

struct Button
{
    int id = -1;
    bool active = true;
    int x = 0;
    int y = 0;
    int w = 0;
    int h = 0;
    const char* text = nullptr;

    SDL_Color outline_color = {255, 255, 255, 255};
    SDL_Color text_color = {255, 255, 255, 255};

    Button(int id, int x, int y, int w, int h, const char* text);
    
    void Draw(SDL_Renderer* renderer, TTF_Font* font);
    bool IsPressed(TTF_Font* font);
    int GetWidth(TTF_Font* font);
    int GetHeight(TTF_Font* font);
};