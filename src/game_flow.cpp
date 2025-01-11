#include "game_flow.h"
#include <SDL2/SDL.h>
#include <iostream>

const int FRAME_DELAY = 700;

GameFlow::GameFlow(Display& display, State game_state, SDL_Renderer* renderer) 
    : display(display),
      state(game_state),
      renderer(renderer),
      running(true),
      game_time(0)
{
    display.reset_background();
    switch (state) {
        case InternGame:
            slug_count = 5;
            int start_y = 40;
            int interval_y = 20;
            slugs[0] = {AI_Daniel, SlugState::MOVING, 98, start_y, 0};
            slugs[1] = {AI_Benson, SlugState::MOVING, 104, start_y + interval_y, 0};
            slugs[2] = {AI_Wes, SlugState::MOVING, 92, start_y + interval_y * 2, 0};
            slugs[3] = {DS, SlugState::MOVING, 106, start_y + interval_y * 3, 0};
            slugs[4] = {DS, SlugState::MOVING, 106, start_y + interval_y * 4, 0};
    }
}

GameFlow::~GameFlow() {}

void GameFlow::calc_frame(Slug& slug) {
    switch (slug.state) {
        case SlugState::MOVING:
            slug.frame++;
            slug.frame %= 2;
            break;
    }
}

void GameFlow::loop() {
    SDL_Event event;

    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            }
            if (event.type == SDL_KEYDOWN) {
                if (event.key.keysym.sym == SDLK_ESCAPE) {
                    running = false;
                }
            }
        }

        switch (state) {
            case InternGame:        
                display.display_background();
                display.add_background_offset(3);
                for (int i = 0; i < slug_count; i++) {
                    calc_frame(slugs[i]);
                    display.display_slug(slugs[i].type, slugs[i].x, slugs[i].y, slugs[i].frame, slugs[i].state);
                }
                SDL_RenderPresent(renderer);
                break;
        }

        SDL_Delay(FRAME_DELAY);
    }
}