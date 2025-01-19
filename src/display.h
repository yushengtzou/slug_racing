#pragma once
#include <SDL2/SDL.h>
#include "slug_enum.h"
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>

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
    void add_background_offset(int move_x);

    void display_countdown(int count);
    void display_text(std::string text, int x, int y, int size, SDL_Color color, int alpha);

    void reset_elements();

    SDL_Texture* get_slug_texture(SlugType slug);
    SDL_Texture* get_end_pattern_texture(bool isGreen);
    SDL_Texture* get_food_texture();
    SDL_Texture* get_prize_tag_texture();
    SDL_Texture* get_effect_texture();
private:
    SDL_Renderer* renderer;
    SDL_Texture* background_texture;
    static int background_width;
    static int background_height;
    SDL_Texture* slug_textures[200];
    SDL_Texture* effect_texture;
    SDL_Texture* end_pattern_textures[2];
    SDL_Texture* food_texture;
    SDL_Texture* prize_tag_texture;
};
