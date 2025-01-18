#include "display.h"
#include <iostream>

int Display::background_offset = 0;
int Display::background_width = 288;
int Display::background_height = 144;
int Display::window_width = 216;
int Display::window_height = 144;

Display::Display(SDL_Renderer* renderer, int height) : renderer(renderer) {
    screen_height = height;
    zoom_factor = height / Display::window_height;

    reset_background();
    reset_slugs();
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

void Display::reset_slugs() {

    SDL_Surface* surfaces[100];
    surfaces[0] = SDL_LoadBMP("../assets/slug_0.bmp");
    surfaces[1] = SDL_LoadBMP("../assets/slug_1.bmp");
    surfaces[2] = SDL_LoadBMP("../assets/slug_3.bmp");
    surfaces[3] = SDL_LoadBMP("../assets/slug_4.bmp");
    
    for (int i = 0; i < 4; i++) {
        slug_textures[i] = SDL_CreateTextureFromSurface(renderer, surfaces[i]);
        SDL_FreeSurface(surfaces[i]);
        if (!slug_textures[i]) {
            std::cerr << "Failed to create texture: " << SDL_GetError() << std::endl;
            return;
        }
    }
    
    SDL_QueryTexture(slug_textures[0], NULL, NULL, NULL, NULL);
}

SDL_Texture* Display::get_slug_texture(SlugType slug) {
    return slug_textures[slug];
}

