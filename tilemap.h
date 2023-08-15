#pragma once

struct Tilemap
{
    int** tiles = nullptr; //IDs linked to Tile objects
    int width = 0;
    int height = 0;
    bool changed_this_frame = false;

    Tilemap(int width, int height); //CAN'T CHANGE WIDTH AND HEIGHT AFTER INITIALIZATION
    ~Tilemap();

    bool Set(int x, int y, int id); //Maybe set and get aren't needed but dealing with a 
                                    //2D Pointer array is a bit ugly
    int Get(int x, int y);

    void Resize(int width, int height);

    void Save(const char* path); //save to a file

    bool Load(const char* path); //load to a file
};