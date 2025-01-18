#include "game_flow.h"
#include <SDL2/SDL.h>
#include <iostream>

const int GameFlow::start_x = 50;
const int GameFlow::fastest_x = 70;

const int FRAME_DELAY = 700;

GameFlow::GameFlow(Display& display, State game_state, SDL_Renderer* renderer) 
    : display(display),
      state(game_state),
      renderer(renderer),
      running(true),
      game_time(0)
{
    srand(time(nullptr));
    display.reset_background();
    cam_move_x = 0;

    int start_y = 40;
    int interval_y = 20;

    switch (state) {
        case EmployeeGame:
            line_count = 5;
            slugs[0] = std::make_unique<Slug>(renderer, display.get_slug_texture(AI_Wes), display.zoom_factor, AI_Wes, GameFlow::start_x, start_y, SlugState::MOVING, 2.0);
            slugs[1] = std::make_unique<Slug>(renderer, display.get_slug_texture(DS), display.zoom_factor, DS, GameFlow::start_x, start_y + interval_y, SlugState::MOVING, 2.0);
            slugs[2] = std::make_unique<Slug>(renderer, display.get_slug_texture(MM_YuMing), display.zoom_factor, MM_YuMing, GameFlow::start_x, start_y + interval_y * 2, SlugState::MOVING, 1.0);
            slugs[3] = std::make_unique<Slug>(renderer, display.get_slug_texture(MM_MuHuan), display.zoom_factor, MM_MuHuan, GameFlow::start_x, start_y + interval_y * 3, SlugState::MOVING, 1.0);
            slugs[4] = std::make_unique<Slug>(renderer, display.get_slug_texture(MM_Lulu), display.zoom_factor, MM_Lulu, GameFlow::start_x, start_y + interval_y * 4, SlugState::MOVING, 3.0);
            break;
        case InternGame:
            line_count = 2;
            slugs[0] = std::make_unique<Slug>(renderer, display.get_slug_texture(AI_Daniel), display.zoom_factor, AI_Daniel, GameFlow::start_x, 80, SlugState::MOVING, 1.0);
            slugs[1] = std::make_unique<Slug>(renderer, display.get_slug_texture(AI_Benson), display.zoom_factor, AI_Benson, GameFlow::start_x, 130, SlugState::MOVING, 1.0);
            break;
        case PrizeIntro:
        case End:
            line_count = 0;
            break;
    }
}

GameFlow::~GameFlow() {}

void GameFlow::loop() {
    SDL_Event event;
    long long game_time = 0;

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

        if (game_time <= 3) {

        }

        else if (game_time > 3 && game_time < 7) {

        }

        else if (game_time >= 7) {
            // Calculate speeds and find fastest slug
            int fastest_slug_index = 0;
            for (int i = 0; i < line_count; i++) {
                if (slugs[i]->state == SlugState::MOVING) {
                    const double max_random = 3.0;
                    double random_factor = static_cast<double>(rand()) / RAND_MAX * max_random;
                    slugs[i]->speed = static_cast<int>(1.0 * (slugs[i]->year_boost * slugs[i]->year_boost_factor + 
                                                            slugs[i]->food_boost * slugs[i]->food_boost_factor) * 
                                                    random_factor);
                    slugs[i]->total_distance += slugs[i]->speed;
                }
                // Track fastest slug
                if (slugs[i]->total_distance > slugs[fastest_slug_index]->total_distance) {
                    fastest_slug_index = i;
                }
            }

            // Update positions relative to fastest slug
            double fastest_distance = slugs[fastest_slug_index]->total_distance;
            if (slugs[fastest_slug_index]->total_distance < GameFlow::fastest_x - GameFlow::start_x) {
                for (int i = 0; i < line_count; i++) {
                    slugs[i]->x = (static_cast<int>(slugs[i]->x + slugs[i]->speed));
                }
            }
            else {
                for (int i = 0; i < line_count; i++) {
                    if (i == fastest_slug_index) {
                        slugs[i]->x = GameFlow::fastest_x;
                    }
                    else {
                        slugs[i]->x = (static_cast<int>(GameFlow::fastest_x - fastest_distance + slugs[i]->total_distance));
                    }
                }
            }

            // Camera moves with fastest slug's speed
            cam_move_x = slugs[fastest_slug_index]->speed;
            
            // Move background with camera
            display.add_background_offset(cam_move_x);
            
            // Move foods with camera
            for (int i = 0; i < line_count; i++) {
                for (auto& food : foods[i]) {
                    food->move_left(cam_move_x);
                }
            }
        }
        game_time++;

        SDL_Delay(FRAME_DELAY);
    }
}

void GameFlow::display_all() {
    switch (state) {
        case InternGame:
        case EmployeeGame:
            display.display_background();
            
            for (int i = 0; i < line_count; i++) {
                for (auto& food : foods[i]) {
                    food->display();
                }
                slugs[i]->display();
            }
            display.fill_bottom();
            SDL_RenderPresent(renderer);
            break;
    }
}
