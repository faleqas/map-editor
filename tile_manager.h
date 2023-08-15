#pragma once
#include "tile.h"

enum
{
    TILE_NONE,
    TILE_TEST,
    TILE_TEST2,
    TILE_COUNT
};

//Once a tile is linked to a manager by AddTile(). it will be deleted if you delete the manager
//Tile objects ids are set by the creator.
struct TileManager
{
    Tile** tiles = nullptr;
    int tile_count = 0;
    int current_tile_index = TILE_TEST;

    TileManager();
    ~TileManager();

    void AddTile(Tile* tile);
    Tile* GetTile(int id);
    void SetCurrentTile(int index); //tile currently used or picked
};