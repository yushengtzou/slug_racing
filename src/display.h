#pragma once
#include <SDL2/SDL.h>
#include "slug_enum.h"

class Display {
public:
    Display(SDL_Renderer* renderer, int screen_height);
    ~Display();

    static int window_width;
    static int window_height;

    int screen_height;
    int zoom_factor;

    static int background_offset;
    void reset_background();
    void display_background();
    void add_background_offset(int move_x);

    static int slug_width;
    static int slug_height;
    void reset_slugs();
    void display_slug(SlugType slug, int x, int y, int frame, SlugState state);

private:
    SDL_Renderer* renderer;
    SDL_Texture* background_texture;
    static int background_width;
    static int background_height;
    SDL_Texture* slug_textures[200];
};
