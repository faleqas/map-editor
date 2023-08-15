#include "tile_manager.h"

TileManager::TileManager()
{

}

TileManager::~TileManager()
{
    if (tiles)
    {
        for (int i = 0; i < tile_count; i++)
        {
            if (tiles[i])
            {
                delete tiles[i];
            }
        }
        delete tiles;
    }
}

void TileManager::AddTile(Tile* tile)
{
    if (tile)
    {
        tile->manager = this;
        tile_count++;

        if (tiles)
        {
            tiles = (Tile**)realloc(tiles, sizeof(Tile*) * tile_count);
        }
        else
        {
            tiles = (Tile**)calloc(tile_count, sizeof(Tile*));
        }
        tiles[tile_count-1] = tile;
    }
}

Tile* TileManager::GetTile(int id)
{
    for (int i = 0; i < tile_count; i++)
    {
        if (tiles[i]->id == id)
        {
            return tiles[i];
        }
    }
    return nullptr;
}

void TileManager::SetCurrentTile(int index)
{
    if (GetTile(index))
    {
        current_tile_index = index;
    }
}