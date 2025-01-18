#include "slug.h"

int Slug::display_width = 48;
int Slug::display_height = 32;
int Slug::anchor_x = 48/2;
int Slug::anchor_y = 32/2;

Slug::Slug(SDL_Renderer* renderer, SDL_Texture* texture, int zoom_factor, SlugType type, int x, int y, SlugState state, int year_boost) {
    this->renderer = renderer;
    this->texture = texture;
    this->zoom_factor = zoom_factor;
    this->type = type;  
    this->x = x;
    this->y = y;
    this->frame = 0;
    this->state = state;
    this->bump_width = 26;
    this->bump_height = 8;
    this->year_boost = 1;
    this->food_boost = 1;
    this->speed_base = 1.0;
}

void Slug::display() {

    // rewrite!!
    frame = (frame + 1) % 2;

    SDL_Rect src_rect = {
        frame * display_width, state * display_height,
        display_width, display_height
    };

    SDL_Rect dest_rect = {
        (x - anchor_x) * zoom_factor, (y - anchor_y) * zoom_factor,
        display_width * zoom_factor, display_height * zoom_factor
    };

    SDL_RenderCopy(renderer, texture, &src_rect, &dest_rect);
}

void Slug::change_speed(string cause, double num = 1){
    switch (cause){
        case "return":
            food_boost = 1;
            break;
        case "year":
            year_boost = num;
            break;
        case "red":
            food_boost = 3;
            break;
        case "green":
        case "blue":
            food_boost = 2;
            break;
        case "poop":
            food_boost = 0;
            break;
    }
    speed_base = year_boost * year_boost_factor * food_boost * food_boost_factor;
}

bool Slug::isBumped(Element* element) {
    int box1_left = x - bump_width / 2;
    int box1_right = x + bump_width / 2;
    int box1_top = y - bump_height / 2;
    int box1_bottom = y + bump_height / 2;

    int box2_left = element->x - element->bump_width / 2;
    int box2_right = element->x + element->bump_width / 2;
    int box2_top = element->y - element->bump_height / 2;
    int box2_bottom = element->y + element->bump_height / 2;

    return (box1_left < box2_right && box1_right > box2_left && box1_top < box2_bottom && box1_bottom > box2_top);
}

