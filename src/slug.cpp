#include "slug.h"

int Slug::display_width = 48;
int Slug::display_height = 32;
int Slug::anchor_x = 48/2;
int Slug::anchor_y = 32/2;

Slug::Slug(SDL_Renderer* renderer, SDL_Texture* texture, int zoom_factor, SlugType type, int x, int y, double year_boost)
    : renderer(renderer),
      texture(texture),
      zoom_factor(zoom_factor),
      type(type),
      speed(0),
      state_record(SlugState::MOVING),
      year_boost(year_boost),
      food_boost(1),
      rank(-1)
{
    this->x = x;
    this->y = y;
    this->bump_width = 26;
    this->bump_height = 8;
}

void Slug::display() {
    int col = 0; // state_record.state

    SDL_Rect src_rect = {
        state_record.frame * display_width, col * display_height,
        display_width, display_height
    };

    SDL_Rect dest_rect = {
        (x - anchor_x) * zoom_factor, (y - anchor_y) * zoom_factor,
        display_width * zoom_factor, display_height * zoom_factor
    };

    SDL_RenderCopy(renderer, texture, &src_rect, &dest_rect);
}

void Slug::change_speed_factors(std::string cause, double num) {
    if (cause == "return") {
        food_boost = 0;
    }
    else if (cause == "year") {
        year_boost = num;
    }
    else if (cause == "red" || cause == "green" || cause == "blue") {
        food_boost = num;
    }
    else if (cause == "poop") {
        food_boost = 0;
    }
}

void Slug::move_total_x(int x) {
    this->total_distance += x;
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

