#include <SDL.h>
#include "cursor.h"
#include "tilemap.h"
#include "tile.h"
#include "tile_manager.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SDL_ttf.h>
#include "button.h"
#include "main.h"
#include "ui_manager.h"

char* RemoveSpaceAndNewline(char* str);
void LoadTiles(SDL_Renderer* renderer, TileManager* mng, const char* path);
char** SplitString(char* str, char delimiter, int* out_len);

int main(int argc, char* argv[])
{
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER);

    SDL_Window* window = SDL_CreateWindow("Editor", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                                          WINDOW_WIDTH, WINDOW_HEIGHT, 0);
    
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 0);

    bool running = true;

    Tilemap tilemap = Tilemap(TILES_WIDTH, TILES_HEIGHT);

    TileManager tile_manager = TileManager();
    //tile_manager.AddTile(new Tile(renderer, TEST_TILE,
    //"C:\\Users\\admin\\source\\repos\\Map-editor\\Map-editor\\abdo.jfif"));

    LoadTiles(renderer, &tile_manager,
    "tiles.txt");

    printf("%d\n", TTF_Init());
    TTF_Font* font = TTF_OpenFont("CONSOLA.ttf", 24);
    printf("%p\n", font);

    UIManager ui_manager = UIManager();

    {
        int x = 0;

        Button* button = new Button(BUTTON_OPEN, x, 0, 0, 0, "Open");

        x += button->GetWidth(font) + TILE_WIDTH / 3;
        Button* button2 = new Button(BUTTON_SAVE, x, 0, 0, 0, "Save");

        x += button2->GetWidth(font) + TILE_WIDTH / 3;
        Button* button3 = new Button(BUTTON_LIST_TILES, x, 0, 0, 0, "List Tiles");

        x += button3->GetWidth(font) + TILE_WIDTH / 3;
        Button* button4 = new Button(BUTTON_CHANGE_INPUT, x, 0, 0, 0, "Mouse/Keyboard");

        x += button4->GetWidth(font) + TILE_WIDTH / 3;
        Button* button5 = new Button(BUTTON_MAP_WIDTH, x, 0, 0, 0, "Width");

        x += button5->GetWidth(font) + TILE_WIDTH / 3;
        Button* button6 = new Button(BUTTON_MAP_HEIGHT, x, 0, 0, 0, "Height");

        ui_manager.AddButton(button);
        ui_manager.AddButton(button2);
        ui_manager.AddButton(button3);
        ui_manager.AddButton(button4);
        ui_manager.AddButton(button5);
        ui_manager.AddButton(button6);
    }

    bool do_draw = true;

    while (running)
    {
        int tick = SDL_GetTicks();

        SDL_Event e;
        SDL_PollEvent(&e);

        switch (e.type)
        {
            case SDL_QUIT:
            {
                running = false;
            } break;

            case SDL_KEYDOWN:
            {
                ui_manager.OnKeyPress(e, &tile_manager, &tilemap);
                do_draw = true;
            } break;

            case SDL_TEXTINPUT:
            {
                ui_manager.OnKeyPress(e, &tile_manager, &tilemap);
                do_draw = true;
            } break;

            case SDL_MOUSEBUTTONDOWN:
            {
                ui_manager.Update(font, &tile_manager, &tilemap);
                do_draw = true;
            } break;

            case SDL_MOUSEMOTION:
            {
                ui_manager.Update(font, &tile_manager, &tilemap);
                do_draw = true;
            } break;
        }

        if (do_draw)
        {
            ui_manager.Draw(renderer, font, &tile_manager, &tilemap);
            
            SDL_RenderPresent(renderer);
        }

        do_draw = false;
    }


    TTF_CloseFont(font);
    TTF_Quit();

    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}


void DrawText(SDL_Renderer* renderer, TTF_Font* font, const char* text, int x, int y, int r, int g, int b)
{
    if (!text)
    {
        return;
    }
    SDL_Color color = {r, g, b};
    SDL_Surface* surface = TTF_RenderText_Solid(font, text, color);
    SDL_Texture * texture = SDL_CreateTextureFromSurface(renderer, surface);

    int width = 0;
    int height = 0;
    SDL_QueryTexture(texture, NULL, NULL, &width, &height);
    SDL_Rect dest = { x, y, width, height };

    	
    SDL_RenderCopy(renderer, texture, NULL, &dest);

    SDL_DestroyTexture(texture);
    SDL_FreeSurface(surface);
}


void LoadTiles(SDL_Renderer* renderer, TileManager* mng, const char* path)
{
    FILE* fp; 
    fopen_s(&fp, path, "r");

    char buffer[255] = {};

    int tile_id = -1;
    const char tile_path[255] = {};

    while (fgets(buffer, 255, fp))
    {
        char* str = RemoveSpaceAndNewline(buffer);
        //printf("%s", str);
        int len = 0;
        char** args = SplitString(str, '=', &len);

        if (len == 2)
        {

            if (strcmp(args[0], "ID") == 0)
            {
                tile_id = strtol(args[1], nullptr, 0);
            }

            else if (strcmp(args[0], "PATH") == 0)
            {
                strcpy_s((char*)tile_path, 255, args[1]);
            }
        }
        else
        {
            if (strcmp(str, "}") == 0)
            {
                if (tile_id > 0)
                {
                    mng->AddTile(new Tile(renderer, tile_id, tile_path));
                    printf("loaded tile with id %d\n", tile_id);
                }
                tile_id = -1;
            }
        }

        for (int i = 0; i < len; i++) {free(args[i]);}
        free(args);
        free(str);
    }

    fclose(fp);
}

char* RemoveSpaceAndNewline(char* str)
{
    int len = strlen(str);
    int len_no_space = 0;

    for (int i = 0; i < len; i++)
    {
        if ((str[i] != ' ') && (str[i] != '\n'))
        {
            len_no_space++;
        }
    }
    len_no_space++; //null character
    char* result = (char*)calloc(len_no_space, sizeof(char));

    int j = 0;
    for (int i = 0; i < len; i++)
    {
        if ((str[i] != ' ') && (str[i] != '\n'))
        {
            result[j] = str[i];
            j++;
        }
    }

    return result;
}

char** SplitString(char* str, char delimiter, int* out_len)
{
    int len = strlen(str);
    int string_count = 0; //amonut of strings seperated in str
    for (int i = 0; i < len; i++)
    {
        if (str[i] == delimiter)
        {
            string_count++;
        }
    }

    if (string_count)
    {
        char** result = (char**)calloc(string_count+1, sizeof(char*));

        int current_str = 0;
        int str_len = 0;
        for (int i = 0; i < len; i++)
        {
            if (str[i] == delimiter)
            {
                str_len = 0;
                current_str++;
                continue;
            }
            str_len++;
            if (result[current_str])
            {
                result[current_str] = (char*)realloc(result[current_str],
                                              sizeof(char) * (str_len+1));
                
            }
            else
            {
                result[current_str] = (char*)calloc(str_len+1, sizeof(char));
            }
            result[current_str][str_len] = 0;

            result[current_str][str_len-1] = str[i];
        }
        *out_len = string_count+1;
        return result;
    }
    return nullptr;
}