#include "display.h"
#include <iostream>

int Display::background_offset = 0;
int Display::background_width = 288;
int Display::background_height = 144;
int Display::window_width = 216;
int Display::window_height = 144;

Display::Display(SDL_Renderer* renderer, int width, int height) : renderer(renderer) {
    zoom_factor = width / Display::window_width;
    std::cout << "width ratio: " << width/Display::window_width << ", height ratio: " << height/Display::window_height << std::endl;

    reset_background();
    reset_elements();
}

Display::~Display() {
    if (background_texture) {
        SDL_DestroyTexture(background_texture);
    }
}

void Display::reset_background() {
    background_offset = 0;

    SDL_Surface* surface = SDL_LoadBMP("../assets/bg_seafloor.bmp");
    if (!surface) {
        std::cerr << "Failed to load seafloor.bmp: " << SDL_GetError() << std::endl;
        return;
    }
    
    background_texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    
    if (!background_texture) {
        std::cerr << "Failed to create texture: " << SDL_GetError() << std::endl;
        return;
    }
    
    SDL_QueryTexture(background_texture, NULL, NULL, &background_width, &background_height);
}

void Display::add_background_offset(int move_x) {
    background_offset -= move_x;
    
    if (background_offset <= -background_width) {
        background_offset = 0;
    }
    if (background_offset > 0) {
        background_offset = -background_width;
    }
}

void Display::display_background() {
    SDL_Rect src_rect = {
        0, 0,
        background_width, background_height
    };
    
    SDL_Rect dest_rect = {
        background_offset * zoom_factor, 0,
        background_width * zoom_factor, background_height * zoom_factor
    };
    
    SDL_Rect dest_rect_cont = {
        (background_offset + background_width) * zoom_factor, 0,
        background_width * zoom_factor, background_height * zoom_factor
    };
    
    SDL_RenderCopy(renderer, background_texture, &src_rect, &dest_rect);
    SDL_RenderCopy(renderer, background_texture, &src_rect, &dest_rect_cont);
}

void Display::display_countdown(int count) {
    std::cout << "display_countdown: " << count << std::endl;
    SDL_Surface* surface = SDL_LoadBMP("../assets/321.bmp");
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    SDL_Rect src_rect = {count * 64, 0, 64, 144};
    // currently screen but should be window for correct zoom
    SDL_Rect dest_rect = {100 * zoom_factor, 0, 64*zoom_factor, 144*zoom_factor};
    SDL_RenderCopy(renderer, texture, &src_rect, &dest_rect);
}

void Display::fill_bottom() {
    // bottom
    SDL_Rect rect = {0, window_height * zoom_factor, 300000, 200000};
    // right
    SDL_Rect rect2 = {window_width * zoom_factor, 0, 100000, 200000};
    SDL_RenderFillRect(renderer, &rect);
    // SDL_RenderFillRect(renderer, &rect2);
}

void Display::reset_elements() {

    SDL_Surface* surfaces[100];
    for (int i = 0; i < 7; i++) {
        std::string filename = "../assets/slug_" + std::to_string(i) + ".bmp";
        surfaces[i] = SDL_LoadBMP(filename.c_str());
        slug_textures[i] = SDL_CreateTextureFromSurface(renderer, surfaces[i]);
        SDL_FreeSurface(surfaces[i]);
        if (!slug_textures[i]) {
            std::cerr << "Failed to create texture: " << SDL_GetError() << std::endl;
            return;
        }
    }

    SDL_Surface* surface = SDL_LoadBMP("../assets/start_and_end.bmp");
    SDL_Surface* surface2 = SDL_LoadBMP("../assets/start_and_end2.bmp");
    end_pattern_textures[0] = SDL_CreateTextureFromSurface(renderer, surface);
    end_pattern_textures[1] = SDL_CreateTextureFromSurface(renderer, surface2);
    SDL_FreeSurface(surface);
    SDL_FreeSurface(surface2);

    SDL_Surface* food_surface = SDL_LoadBMP("../assets/food.bmp");
    food_texture = SDL_CreateTextureFromSurface(renderer, food_surface);
    SDL_FreeSurface(food_surface);
}

SDL_Texture* Display::get_slug_texture(SlugType slug) {
    return slug_textures[slug];
}

SDL_Texture* Display::get_food_texture() {
    return food_texture;
}

SDL_Texture* Display::get_end_pattern_texture(bool isGreen) {
    return end_pattern_textures[isGreen];
}

