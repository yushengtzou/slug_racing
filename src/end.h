#pragma once
#include <SDL2/SDL.h>
#include "element.h"
#include "display.h"

class End : public Element {
public:
    End(SDL_Renderer* renderer, Display* disp) {
        this->renderer = renderer;
        this->disp = disp;
        this->topleft_x = 0;
        this->topleft_y = 0;
        this->x = topleft_x + tile_size * tile_row / 2;
        this->y = topleft_y + tile_size * tile_col / 2;
        this->bump_width = tile_size * tile_row;
        this->bump_height = tile_size * tile_col;
    }
    void display() override {
        for (int i = 0; i < tile_row; i++) {
            for (int j = 0; j < tile_col; j++) {
                SDL_Rect src_rect = {
                    0, 0,
                    tile_size, tile_size
                };
                SDL_Rect dest_rect = {
                    (x + i * tile_size) * disp->zoom_factor, 
                    (j * tile_size) * disp->zoom_factor,
                    tile_size * disp->zoom_factor, 
                    tile_size * disp->zoom_factor
                };
                SDL_RenderCopy(renderer, disp->get_end_pattern_texture((i+j) % 2 == 0), &src_rect, &dest_rect);
            }
        }
    }

    int topleft_x, topleft_y;
    const int tile_size = 24;
    const int tile_row = 2;
    const int tile_col = 144/24;
    const int tile_num = tile_row * tile_col;

private:
    SDL_Renderer* renderer;
    Display* disp;
};
