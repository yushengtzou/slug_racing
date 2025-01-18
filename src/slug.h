#pragma once
#include <SDL2/SDL.h>
#include "element.h"
#include "slug_enum.h"

class Slug : public Element {
public:
    Slug(SDL_Renderer* renderer, SDL_Texture* texture, int zoom_factor, SlugType type, int x, int y, SlugState state, double year_boost);
    void display() override;
    void change_speed_factors(std::string cause, double num);
    bool isBumped(Element* element);

    SlugType type;
    int frame, speed;
    SlugState state;

    int rank;

    static constexpr double year_boost_factor = 1.05, food_boost_factor = 1.5;
    double year_boost; // 1, 2, 3
    double food_boost; // 0, 1, 2, 3

    static int display_width, display_height, anchor_x, anchor_y;

    long long total_distance;
    void move_total_x(int x);

private:
    int zoom_factor;
    SDL_Renderer* renderer;
    SDL_Texture* texture;
};
