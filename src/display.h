#pragma once
#include <SDL2/SDL.h>
#include "slug_enum.h"

class Display {
public:
    Display(SDL_Renderer* renderer, int screen_width, int screen_height);
    ~Display();
    void fill_bottom();

    static int window_width;
    static int window_height;

    int zoom_factor;

    static int background_offset;
    void reset_background();
    void display_background();
    void display_countdown(int count);
    void add_background_offset(int move_x);

    void reset_elements();

    SDL_Texture* get_slug_texture(SlugType slug);
    SDL_Texture* get_end_pattern_texture(bool isGreen);
    SDL_Texture* get_food_texture();

private:
    SDL_Renderer* renderer;
    SDL_Texture* background_texture;
    static int background_width;
    static int background_height;
    SDL_Texture* slug_textures[200];
    SDL_Texture* end_pattern_textures[2];
    SDL_Texture* food_texture;
};
