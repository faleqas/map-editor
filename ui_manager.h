#pragma once
#include <SDL.h>
#include "button.h"
#include "cursor.h"
#include "tile_manager.h"
#include "tilemap.h"

enum
{
    UISTATE_OPTIONS,
    UISTATE_GETINPUT,
    UISTATE_LIST_TILES
};

struct UIManager
{
    Button** buttons = nullptr;
    int button_count = 0;
    int state = UISTATE_OPTIONS;
    Button* current_button = nullptr;//last pressed button
    char input[255] = {};
    int input_len = 0;
    bool use_mouse = true;
    int camera_x = 0; //in tiles
    int camera_y = 0;
    Cursor cursor;

    UIManager();
    ~UIManager();

    void AddButton(Button* button);
    void Update(TTF_Font* font, TileManager* tile_manager, Tilemap* tilemap);
    void Draw(SDL_Renderer* renderer, TTF_Font* font, TileManager* tile_manager, Tilemap* tilemap);
    void OnKeyPress(SDL_Event e, TileManager* tile_manager, Tilemap* tilemap);
};