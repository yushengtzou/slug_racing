#pragma once
#include <SDL2/SDL.h>
#include "element.h"
#include "display.h"
#include "slug_enum.h"
class Food : public Element {
public:
    Food(SDL_Renderer* renderer, Display* disp, FoodType food_type, int x, int y) {
        this->renderer = renderer;
        this->disp = disp;
        this->food_type = food_type;
        this->x = x;
        this->y = y;
        this->bump_width = 24;
        this->bump_height = 32;
    }
    void display() override {
        SDL_Rect src_rect = {
            food_type * display_width, 0,
            display_width, display_height
        };
        SDL_Rect dest_rect = {
            (x - display_width/2) * disp->zoom_factor, 
            (y - display_height/2) * disp->zoom_factor,
            display_width * disp->zoom_factor, 
            display_height * disp->zoom_factor
        };
        SDL_RenderCopy(renderer, disp->get_food_texture(), &src_rect, &dest_rect);
    }

    FoodType food_type;

    static const int display_width = 48;
    static const int display_height = 32;

private:
    SDL_Renderer* renderer;
    Display* disp;
};
