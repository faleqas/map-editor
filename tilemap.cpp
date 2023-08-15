#include "tilemap.h"
#include <stdlib.h>
#include <stdio.h>
#include <memory.h>

Tilemap::Tilemap(int width, int height)
{
    tiles = (int**)calloc(height, sizeof(int*));
    for (int i = 0; i < height; i++)
    {
        tiles[i] = (int*)calloc(width, sizeof(int));
    }
    this->width = width;
    this->height = height;
}

Tilemap::~Tilemap()
{
    for (int i = 0; i < height; i++)
    {
        free(tiles[i]);
    }
    free(tiles);
}

bool Tilemap::Set(int x, int y, int id)
{
    if ((x >= 0) && (x < width))
    {
        if ((y >= 0) && (y < height))
        {
            tiles[y][x] = id;
            changed_this_frame = true;
            return true;
        }
    }
    return false;
}

int Tilemap::Get(int x, int y)
{
    if ((x >= 0) && (x < width))
    {
        if ((y >= 0) && (y < height))
        {
            return tiles[y][x];
        }
    }
    return -1;
}

void Tilemap::Save(const char* path)
{
    FILE* fp;
    fopen_s(&fp, path, "wb");

    fseek(fp, 0, 0);

    _putw(width, fp); //width is the first int in file
    _putw(height, fp);

    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            int i = tiles[y][x];
            _putw(i, fp);
        }
    }

    fclose(fp);
}

bool Tilemap::Load(const char* path)
{
    FILE* fp;
    fopen_s(&fp, path, "rb");

    if (!fp)
    {
        return false;
    }

    fseek(fp, 0, 0);

    int width = _getw(fp);
    int height = _getw(fp);

    if (!width)
    {
        return false;
    }
    if (!height)
    {
        return false;
    }

    if (tiles)
    {
        for (int i = 0; i < this->height; i++)
        {
            free(tiles[i]);
        }
        free(tiles);
        tiles = nullptr;
    }

    this->height = height;
    this->width = width;

    tiles = (int**)calloc(height, sizeof(int*));
    for (int i = 0; i < height; i++)
    {
        tiles[i] = (int*)calloc(width, sizeof(int));
    }

    int x = 0;
    int y = 0;

    while (!feof(fp))
    {
        int i = _getw(fp);
        if (x >= width)
        {
            x = 0;
            y++;
        }
        if (y >= height)
        {
            y = 0; //Would cause weird behavior. give out an error and abort the loading instead
        }
        tiles[y][x] = i;
        x++;
    }

    return true;
}

void Tilemap::Resize(int width, int height)
{
    tiles = (int**)realloc(tiles, sizeof(int*) * height);

    for (int y = this->height; y < height; y++)
    {
        tiles[y] = nullptr;
    }

    for (int y = 0; y < height; y++)
    {
        if (tiles[y])
        {
            tiles[y] = (int*)realloc(tiles[y], sizeof(int) * width);
        }
        else
        {
            tiles[y] = (int*)calloc(width, sizeof(int));
        }
        if (width > this->width)
        {
            memset(tiles[y] + this->width, 0, width - this->width);
        }
    }

    this->width = width;
    this->height = height;
}