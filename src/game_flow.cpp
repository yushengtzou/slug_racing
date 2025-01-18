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
            line_count = 5;
            int start_x = 60;
            int start_y = 40;
            int interval_y = 20;

            slugs[0] = std::make_unique<Slug>(renderer, display.get_slug_texture(AI_Daniel), display.zoom_factor, AI_Daniel, start_x, start_y, SlugState::MOVING, 1.0);
            slugs[1] = std::make_unique<Slug>(renderer, display.get_slug_texture(AI_Benson), display.zoom_factor, AI_Benson, start_x, start_y + interval_y, SlugState::MOVING, 1.0);
            slugs[2] = std::make_unique<Slug>(renderer, display.get_slug_texture(AI_Wes), display.zoom_factor, AI_Wes, start_x, start_y + interval_y * 2, SlugState::MOVING, 1.0);
            slugs[3] = std::make_unique<Slug>(renderer, display.get_slug_texture(DS), display.zoom_factor, DS, start_x, start_y + interval_y * 3, SlugState::MOVING, 1.0);
            slugs[4] = std::make_unique<Slug>(renderer, display.get_slug_texture(DS), display.zoom_factor, DS, start_x, start_y + interval_y * 4, SlugState::MOVING, 1.0);
    }
}

GameFlow::~GameFlow() {}

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

        display_all();

        // event
        for (int i = 0; i < line_count; i++) {

            if (slugs[i]->state == SlugState::MOVING) {
                
            }
        }

        SDL_Delay(FRAME_DELAY);
    }
}

void GameFlow::display_all() {
    switch (state) {
        case InternGame:
        case EmployeeGame:
            display.display_background();
            display.add_background_offset(3);
            
            for (int i = 0; i < line_count; i++) {
                for (auto& food : foods[i]) {
                    food->display();
                }
                slugs[i]->display();
            }
            SDL_RenderPresent(renderer);
            break;
    }
}
