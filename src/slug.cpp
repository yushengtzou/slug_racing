#include "slug.h"

int Slug::display_width = 48;
int Slug::display_height = 32;
int Slug::anchor_x = 48/2;
int Slug::anchor_y = 32/2;

Slug::Slug(SDL_Renderer* renderer, Display* display, SDL_Texture* texture, SDL_Texture* effect_texture, int zoom_factor, SlugType type, int x, int y, double year_boost)
    : renderer(renderer),
      disp(display),
      texture(texture),
      effect_texture(effect_texture),
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
    this->ingame_rank = -1;
    switch(type) {
        case SlugType::AI_Benson: name = "Benson"; break;
        case SlugType::AI_Daniel: name = "Daniel"; break;
        case SlugType::AI_Wes: name = "Wes"; break;
        case SlugType::DS: name = "Tako"; break;
        case SlugType::MM_YuMing: name = "YuMing"; break;
        case SlugType::MM_MuHuan: name = "MuHuan"; break;
        case SlugType::MM_Lulu: name = "Lulu"; break;
    }
}

void Slug::display() {
    // display rank tag
    const int tag_width = 17, tag_height = 9;
    SDL_Rect tag_src_rect;
    if(name == "Daniel" || name == "Benson") {
        tag_src_rect = {
            4 * tag_width, 0,
            tag_width, tag_height
        };
    }
    else {
        tag_src_rect = {
            std::min(3, ingame_rank) * tag_width, 0,
            tag_width, tag_height
        };
    }
    SDL_Rect tag_dest_rect = {
        (x - tag_width/2 + 17) * zoom_factor, 
        (y - tag_height/2 - 8) * zoom_factor,
        tag_width * zoom_factor, 
        tag_height * zoom_factor
    };
    // dont display tag for now
    // SDL_RenderCopy(renderer, disp->get_prize_tag_texture(), &tag_src_rect, &tag_dest_rect);

    // display slug
    int col = 0; // state_record.state
    SDL_Rect src_rect = {
        state_record.frame * display_width, col * display_height,
        display_width, display_height
    };
    SDL_Rect dest_rect = {
        (x - anchor_x) * zoom_factor, (y - anchor_y) * zoom_factor,
        display_width * zoom_factor, display_height * zoom_factor
    };
    if ((state_record.state == SlugState::BOOSTED || state_record.state == SlugState::SUPER_BOOSTED || state_record.state == SlugState::SHIELDED) && state_record.frame == 0) {
        dest_rect.y += 1;
    }
    SDL_RenderCopy(renderer, texture, &src_rect, &dest_rect);

    // effect image
    int effect_col = -1;
    if (state_record.state == SlugState::BOOSTED) effect_col = 0;
    else if (state_record.state == SlugState::SUPER_BOOSTED) effect_col = 1;
    else if (state_record.state == SlugState::SHIELDED) effect_col = 2;
    else if (state_record.state == SlugState::STOPPED) effect_col = 3;
    SDL_Rect effect_src_rect = {
        state_record.frame * display_width, effect_col * display_height,
        display_width, display_height
    };
    if (effect_col != -1) SDL_RenderCopy(renderer, effect_texture, &effect_src_rect, &dest_rect);

    // effect text
    int effect_text_offset_x = 15, effect_text_offset_y = 3;
    int effect_text_duration = 4, effect_text_size = 12;
    SDL_Color effect_text_color = {12, 12, 12, 255};
    if (state_record.state == SlugState::BOOSTED) disp->display_text("加速！", x + effect_text_offset_x, y - effect_text_offset_y, effect_text_size, effect_text_color, fmax(0, double(effect_text_duration - state_record.animation_time) / effect_text_duration) * 255);
    else if (state_record.state == SlugState::SUPER_BOOSTED) disp->display_text("超加速！", x + effect_text_offset_x, y - effect_text_offset_y, effect_text_size, effect_text_color, fmax(0, double(effect_text_duration - state_record.animation_time) / effect_text_duration) * 255);
    else if (state_record.state == SlugState::SHIELDED) disp->display_text("保護！", x + effect_text_offset_x, y - effect_text_offset_y, effect_text_size, effect_text_color, fmax(0, double(effect_text_duration - state_record.animation_time) / effect_text_duration) * 255);
    else if (state_record.state == SlugState::STOPPED) disp->display_text("停滯！", x + effect_text_offset_x, y - effect_text_offset_y, effect_text_size, effect_text_color, fmax(0, double(effect_text_duration - state_record.animation_time) / effect_text_duration) * 255);
}

void Slug::change_speed_factors(std::string cause, double num) {
    if (cause == "return") {
        food_boost = 0;
    }
    else if (cause == "year") {
        year_boost = num;
    }
    else if (cause == "food") {
        food_boost = num;
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

