#include "ui_manager.h"
#include "main.h"
#include <stdio.h>
#include <stdlib.h>

UIManager::UIManager()
{
    cursor.parent = this;
}

UIManager::~UIManager()
{
    cursor.parent = nullptr;
    for (int i = 0; i < button_count; i++)
    {
        if (buttons[i])
        {
            delete buttons[i];
        }
    }
    delete buttons;
}

void UIManager::AddButton(Button* button)
{
    button_count++;
    if (buttons)
    {
        buttons = (Button**)realloc(buttons, sizeof(Button*) * button_count);
    }
    else
    {
        buttons = (Button**)calloc(button_count, sizeof(Button*));
    }
    buttons[button_count-1] = button;
}

void UIManager::Update(TTF_Font* font, TileManager* tile_manager, Tilemap* tilemap)
{
    if (use_mouse)
    {
        int mouse_x = 0;
        int mouse_y = 0;
        int mouse_state = SDL_GetMouseState(&mouse_x, &mouse_y);
        cursor.UpdateToMousePos(mouse_x, mouse_y);
        cursor.tile_x += camera_x;
        cursor.tile_y += camera_y;

        if (mouse_y > DRAWABLE_Y) //is within the edit area
        {
            if (mouse_state & SDL_BUTTON_LMASK)
            {
                tilemap->Set(cursor.tile_x, (cursor.tile_y - (DRAWABLE_Y / TILE_HEIGHT)),
                tile_manager->current_tile_index);
            }
            else if (mouse_state & SDL_BUTTON_RMASK)
            {
                tilemap->Set(cursor.tile_x, (cursor.tile_y - (DRAWABLE_Y / TILE_HEIGHT)),
                TILE_NONE);
            }
        }
    }

    switch (state)
    {
        case UISTATE_OPTIONS:
        {
            for (int i = 0; i < button_count; i++)
            {
                Button* button = buttons[i];
                if (button)
                {
                    if (button->IsPressed(font))
                    {
                        switch (button->id)
                        {
                            case BUTTON_OPEN:
                            {
                                state = UISTATE_GETINPUT;
                                current_button = button;
                            } break;

                            case BUTTON_SAVE:
                            {
                                state = UISTATE_GETINPUT;
                                current_button = button;
                            } break;

                            case BUTTON_LIST_TILES:
                            {
                                state = UISTATE_LIST_TILES;
                                current_button = button;
                            } break;

                            case BUTTON_CHANGE_INPUT:
                            {
                                use_mouse = !use_mouse;
                            } break;

                            case BUTTON_MAP_WIDTH:
                            {
                                state = UISTATE_GETINPUT;
                                current_button = button;
                            } break;

                            case BUTTON_MAP_HEIGHT:
                            {
                                state = UISTATE_GETINPUT;
                                current_button = button;
                            } break;

                            default:
                            {

                            } break;
                        }
                    }
                }
            }
        } break;

        case UISTATE_LIST_TILES:
        {
            int mouse_x = 0;
            int mouse_y = 0;
            int mouse_state = SDL_GetMouseState(&mouse_x, &mouse_y);

            if (mouse_state & SDL_BUTTON_LMASK)
            {
                int tile_x = mouse_x / TILE_WIDTH;
                int tile_y = mouse_y / TILE_HEIGHT;
                int width_in_tiles = WINDOW_WIDTH / TILE_WIDTH;

                int index = tile_x;
                index += tile_y * width_in_tiles;

                if (tile_manager->GetTile(index+1))
                {
                    printf("pressed on tile number %d\n", index);
                    tile_manager->SetCurrentTile(index+1);
                    state = UISTATE_OPTIONS;
                }
            }
        } break;

        default:
        {

        } break;
    }
}

void UIManager::Draw(SDL_Renderer* renderer, TTF_Font* font, TileManager* tile_manager, Tilemap* tilemap)
{
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0xff);
    SDL_RenderClear(renderer);

    SDL_SetRenderDrawColor(renderer, 0xff, 0xff, 0xff, 0xff);
    SDL_Rect menu_rect = {
        0, 0,
        WINDOW_WIDTH,
        DRAWABLE_Y - 1
    };
    SDL_RenderDrawRect(renderer, &menu_rect);
    for (int i = camera_y; i < tilemap->height; i++)
    {
        for (int j = camera_x; j < tilemap->width; j++)
        {
            int id = tilemap->Get(j, i);

            if (id > 0)
            {
                Tile* tile = tile_manager->GetTile(id);
                if (tile)
                {
                    tile->Draw(renderer, j - camera_x, i - camera_y);
                }
            }
        }
    }
    
    switch (state)
    {
        case UISTATE_OPTIONS:
        {
            for (int i = 0; i < button_count; i++)
            {
                if (buttons[i])
                {
                    buttons[i]->Draw(renderer, font);
                }
            }
            //DrawText(renderer, font, "UISTATE_OPTIONS`", 0, DRAWABLE_Y - TILE_HEIGHT / 3, 255, 255, 255);
        } break;

        case UISTATE_GETINPUT:
        {
            //DrawText(renderer, font, "UISTATE_GETINPUT", 0, DRAWABLE_Y - TILE_HEIGHT / 3, 255, 255, 255);
            {
                int w = 0;
                const char* arg = nullptr;
                if (current_button)
                {
                    switch (current_button->id)
                    {
                        case BUTTON_OPEN:
                        case BUTTON_SAVE:
                        {
                            arg = "Filename: ";
                        } break;

                        case BUTTON_MAP_WIDTH:
                        {
                            arg = "Enter Width: ";
                        } break;

                        case BUTTON_MAP_HEIGHT:
                        {
                            arg = "Enter Height: ";
                        } break;
                    };
                }
                TTF_SizeText(font, arg, &w, nullptr);
                DrawText(renderer, font, arg, 0, 0, 255, 255, 255);
                DrawText(renderer, font, input, w, 0, 255, 255, 255);
            }
        } break;

        case UISTATE_LIST_TILES:
        {
            //DrawText(renderer, font, "UISTATE_LIST_TILES", 0, DRAWABLE_Y - TILE_HEIGHT / 3, 255, 255, 255);
            int mouse_x = 0;
            int mouse_y = 0;
            SDL_GetMouseState(&mouse_x, &mouse_y);
            mouse_x /= TILE_WIDTH;
            mouse_y /= TILE_HEIGHT;

            int x = 0;
            int y = 0;
            for (int i = 0; i < TILE_COUNT; i++)
            {
                Tile* tile = tile_manager->GetTile(i);
                if (tile)
                {
                    tile->Draw(renderer, x, y, false);

                    if ((x == mouse_x) && (y == mouse_y))
                    {
                        //draw outline on tile
                        SDL_SetRenderDrawColor(renderer, 0xff, 0xff, 0xff, 0xff);
                        SDL_Rect rect = {
                            x * TILE_WIDTH,
                            y * TILE_HEIGHT,
                            TILE_WIDTH,
                            TILE_HEIGHT
                        };
                        SDL_RenderDrawRect(renderer, &rect);
                    }

                    x++;
                    if (x >= (WINDOW_WIDTH / TILE_WIDTH))
                    {
                        x = 0;
                        y++;
                    }
                }
            }
        }

        default:
        {
            
        } break;
    }

    cursor.Draw(renderer);


    if (state != UISTATE_LIST_TILES)
    {
        if (use_mouse)
        {
            DrawText(renderer, font, "Using Mouse", 5,
            DRAWABLE_Y - TILE_HEIGHT / 2, 255, 0, 0);
        }
        else
        {
            DrawText(renderer, font, "Using Keyboard", 5,
            DRAWABLE_Y - TILE_HEIGHT / 2, 255, 0, 0);
        }

        char buffer[255] = {};
        snprintf(buffer, 255, "Width: %d, Height: %d", tilemap->width, tilemap->height);
        DrawText(renderer, font, buffer, 5, DRAWABLE_Y - TILE_HEIGHT, 255, 0, 0);
    }
}

void UIManager::OnKeyPress(SDL_Event e, TileManager* tile_manager, Tilemap* tilemap)
{
    switch (state)
    {
        case UISTATE_GETINPUT:
        {
            switch (e.type)
            {
                case SDL_TEXTINPUT:
                {
                    if (input_len < 255)
                    {
                        char c = e.text.text[0];
                        input[input_len] = c;
                        input_len++;
                    }
                } break;

                case SDL_KEYDOWN:
                {
                    switch (e.key.keysym.sym)
                    {
                        case SDLK_BACKSPACE:
                        {
                            if (input_len)
                            {
                                input_len--;
                                input[input_len] = 0;
                            }
                        } break;

                        case SDLK_RETURN:
                        {
                            if (current_button)
                            {
                                switch (current_button->id)
                                {
                                    case BUTTON_SAVE:
                                    {
                                        tilemap->Save(input);
                                    } break;

                                    case BUTTON_OPEN:
                                    {
                                        tilemap->Load(input);
                                    } break;

                                    case BUTTON_MAP_WIDTH:
                                    {
                                        int width = atoi(input);
                                        if (width < 8)
                                        {
                                            width = 8;
                                        }
                                        tilemap->Resize(width, tilemap->height);
                                    } break;

                                    case BUTTON_MAP_HEIGHT:
                                    {
                                        int height = atoi(input);
                                        if (height < 8)
                                        {
                                            height = 8;
                                        }
                                        tilemap->Resize(tilemap->width, height);
                                    } break;
                                };
                            }
                            for (int i = 0; i < 255; i++)
                            {
                                input[i] = 0;
                                input_len = 0;
                            }
                            state = UISTATE_OPTIONS;
                        } break;
                        } break;

                        default:
                        {

                        } break;
                    }
                }
        } break;

        case UISTATE_OPTIONS:
        {
            if (!use_mouse)
            {
                cursor.OnKeyPress(e);
            }

            switch (e.key.keysym.sym)
            {
                case SDLK_BACKSPACE:
                {
                    tilemap->Set(cursor.tile_x, (cursor.tile_y),
                    TILE_NONE);
                } break;
                
                case SDLK_RETURN:
                {
                    //PLACE TILE
                    //QUESTION?() Should the UI_Manager really be responsible for this ? i guess

                    if (!use_mouse)
                    {
                    tilemap->Set(cursor.tile_x, (cursor.tile_y - (DRAWABLE_Y / TILE_HEIGHT)),
                    tile_manager->current_tile_index);
                    }
                } break;

                case SDLK_RIGHT:
                {
                    camera_x++;
                    cursor.tile_x++;
                } break;

                case SDLK_LEFT:
                {
                    camera_x--;
                    cursor.tile_x--;
                } break;

                case SDLK_UP:
                {
                    camera_y--;
                    cursor.tile_y--;
                } break;

                case SDLK_DOWN:
                {
                    camera_y++;
                    cursor.tile_y++;
                } break;
            }
        } break;
        case UISTATE_LIST_TILES:
        {
            if (!use_mouse)
            {
                cursor.OnKeyPress(e);
            }

            switch (e.key.keysym.sym)
            {
                case SDLK_BACKSPACE:
                {
                    state = UISTATE_OPTIONS;
                } break;
                
                case SDLK_RETURN:
                {
                    //PLACE TILE
                    //QUESTION?() Should the UI_Manager really be responsible for this ? i guess

                    if (!use_mouse)
                    {
                    tilemap->Set(cursor.tile_x, (cursor.tile_y - (DRAWABLE_Y / TILE_HEIGHT)),
                    tile_manager->current_tile_index);
                    }
                } break;
            }
        } break;

        default:
        {

        } break;
    }
}