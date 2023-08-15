#include "button.h"
#include "main.h"

Button::Button(int id, int x, int y, int w, int h, const char* text)
{
    this->id = id;
    this->x = x;
    this->y = y;
    this->w = w;
    this->h = h;
    this->text = text;
}

void Button::Draw(SDL_Renderer* renderer, TTF_Font* font)
{
    SDL_SetRenderDrawColor(renderer, outline_color.r, outline_color.g, outline_color.b, outline_color.a);
    SDL_Rect rect = {
        x, y,
        GetWidth(font), GetHeight(font)
    };
    SDL_RenderDrawRect(renderer, &rect);

    int text_h = 0;
    TTF_SizeText(font, text, nullptr, &text_h);

    int text_x = x;
    int text_y = y + (rect.h / 2) - (text_h / 2);

    DrawText(renderer, font, text, text_x, text_y, text_color.r, text_color.g, text_color.b);
    
}

int Button::GetWidth(TTF_Font* font)
{
    if (w)
    {
        return w;
    }
    else
    {
        int text_w = 0;
        TTF_SizeText(font, text, &text_w, nullptr);
        return text_w;
    }
}

int Button::GetHeight(TTF_Font* font)
{
    if (h)
    {
        return h;
    }
    else
    {
        int text_h = 0;
        TTF_SizeText(font, text, nullptr, &text_h);
        return text_h * 1.2f;
    }
}

bool Button::IsPressed(TTF_Font* font)
{
    int mouse_x = 0;
    int mouse_y = 0;
    int mouse_state = SDL_GetMouseState(&mouse_x, &mouse_y);

    if (mouse_state & SDL_BUTTON_LMASK)
    {
        if ((mouse_x > x) && (mouse_x < (x + GetWidth(font))))
        {
            if ((mouse_y > y) && (mouse_y < (y + GetHeight(font))))
            {
                return true;
            }
        }
    }
    return false;
}