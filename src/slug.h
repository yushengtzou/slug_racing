#pragma once
#include <SDL2/SDL.h>
#include "element.h"
#include "slug_enum.h"

class Slug : public Element {
public:
    Slug(SDL_Renderer* renderer, SDL_Texture* texture, int zoom_factor, SlugType type, int x, int y, SlugState state, int year_boost);
    void display() override;
    bool isBumped(Element* element);

    SlugType type;
    int frame;
    SlugState state;

    static const double year_boost_factor = 1.15, food_boost_factor = 1.5;
    double year_boost; // 1, 2, 3
    double food_boost; // 0, 1, 2, 3
    double speed_base;

    static int display_width, display_height, anchor_x, anchor_y;

private:
    int zoom_factor;
    SDL_Renderer* renderer;
    SDL_Texture* texture;
};
