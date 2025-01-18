#include "game_flow.h"
#include <SDL2/SDL.h>
#include <iostream>

const int GameFlow::start_x = 50;

const int FRAME_DELAY = 500;

GameFlow::GameFlow(Display& display, State game_state, SDL_Renderer* renderer) 
    : display(display),
      state(game_state),
      renderer(renderer),
      running(true),
      game_time(0),
      end_banner(renderer, &display)
{
    srand(time(nullptr));
    for(auto& food : foods) {
        food = nullptr;
    }

    display.reset_background();
    cam_move_x = 0;
    fastest_x = 75;
    countdown_hold_time = 2;
    all_slugs_reached = false;
    int start_y = 40;
    int interval_y = 20;

    switch (state) {
        case EmployeeGame:
            line_count = 5;
            slugs[0] = std::make_unique<Slug>(renderer, display.get_slug_texture(AI_Wes), display.zoom_factor, AI_Wes, GameFlow::start_x, start_y, 2.0);
            slugs[1] = std::make_unique<Slug>(renderer, display.get_slug_texture(DS), display.zoom_factor, DS, GameFlow::start_x, start_y + interval_y, 2.0);
            slugs[2] = std::make_unique<Slug>(renderer, display.get_slug_texture(MM_YuMing), display.zoom_factor, MM_YuMing, GameFlow::start_x, start_y + interval_y * 2, 1.0);
            slugs[3] = std::make_unique<Slug>(renderer, display.get_slug_texture(MM_MuHuan), display.zoom_factor, MM_MuHuan, GameFlow::start_x, start_y + interval_y * 3, 1.0);
            slugs[4] = std::make_unique<Slug>(renderer, display.get_slug_texture(MM_Lulu), display.zoom_factor, MM_Lulu, GameFlow::start_x, start_y + interval_y * 4, 3.0);
            food_appear_x = 100;
            end_banner_appear_x = 800;
            end_banner.x += end_banner_appear_x;
            show_year_time = 4;
            break;
        case InternGame:
            line_count = 2;
            slugs[0] = std::make_unique<Slug>(renderer, display.get_slug_texture(AI_Daniel), display.zoom_factor, AI_Daniel, GameFlow::start_x, 80, 1.0);
            slugs[1] = std::make_unique<Slug>(renderer, display.get_slug_texture(AI_Benson), display.zoom_factor, AI_Benson, GameFlow::start_x, 130, 1.0);
            food_appear_x = 100;
            end_banner_appear_x = 800;
            end_banner.x += end_banner_appear_x;
            show_year_time = 0;
            break;
        case PrizeIntro:
        case EndAnimation:
            line_count = 0;
            break;
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

        std::cout << "game_time: " << game_time << std::endl;
        display_all();

        if (game_time < show_year_time) {

        }

        else if (game_time < show_year_time + countdown_hold_time * 3) {
            for (int i = 0; i < line_count; i++) {
                slugs[i]->state_record.frame = 0;
            }
        }

        // game logic
        else if (!all_slugs_reached) {
            // Calculate speeds and find fastest slug
            int fastest_slug_index = 0;
            for (int i = 0; i < line_count; i++) {
                if (slugs[i]->rank == -1) {
                    const double max_random = 3.0;
                    if(slugs[i]->state_record.state == SlugState::MOVING) {
                        double random_factor = static_cast<double>(rand()) / RAND_MAX * max_random;
                        slugs[i]->speed = static_cast<int>(1.0 * (slugs[i]->year_boost * slugs[i]->year_boost_factor + 
                                                                slugs[i]->food_boost * slugs[i]->food_boost_factor) * 
                                                        random_factor);
                    }
                    else if(slugs[i]->state_record.state == SlugState::BOOSTED || slugs[i]->state_record.state == SlugState::SHIELDED) {
                        double random_factor = static_cast<double>(rand()) / RAND_MAX * max_random;
                        slugs[i]->speed = static_cast<int>(2.0 * (slugs[i]->year_boost * slugs[i]->year_boost_factor + 
                                                                slugs[i]->food_boost * slugs[i]->food_boost_factor) * 
                                                        random_factor);
                    }
                    else if(slugs[i]->state_record.state == SlugState::STOPPED) {
                        slugs[i]->speed = 0;
                    }
                    slugs[i]->total_distance += slugs[i]->speed;
                    // Track fastest slug
                    if (slugs[i]->total_distance > slugs[fastest_slug_index]->total_distance) {
                        fastest_slug_index = i;
                    }
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
                    if (slugs[i]->rank != -1) {
                        slugs[i]->x = end_banner.x;
                    }
                    else if (i == fastest_slug_index) {
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
                if (foods[i]) {
                    foods[i]->move_left(cam_move_x);
                }
            }

            end_banner.move_left(cam_move_x);

            for (int i = 0; i < line_count; i++) {
                // frame control
                slugs[i]->state_record.frame = (slugs[i]->state_record.frame + 1) % slugs[i]->state_record.frameCount;

                // event animation (e.g. boosted) time control
                if (slugs[i]->state_record.animation_duration != -1) {
                    slugs[i]->state_record.animation_time++;
                    if (slugs[i]->state_record.animation_time >= slugs[i]->state_record.animation_duration) {
                        slugs[i]->state_record.animation_time = 0;
                        slugs[i]->state_record.state = SlugState::MOVING;
                    }
                }

                if (slugs[i]->state_record.state == SlugState::MOVING) {
                    // bump into food?
                    if(foods[i]) std::cout << "food position: " << foods[i]->x << " slug position: " << slugs[i]->x << " food type: " << foods[i]->food_type << " slug state: " << slugs[i]->state_record.state << std::endl;
                    if (foods[i] && slugs[i]->isBumped(foods[i].get())) {
                        std::cout << "==========slug " << i << " bumped into food " << foods[i]->food_type << std::endl;
                        switch (foods[i]->food_type) {
                            case FoodType::RED:
                            case FoodType::GREEN:
                                slugs[i]->state_record.state = SlugState::BOOSTED;
                                break;
                            case FoodType::BLUE:
                                slugs[i]->state_record.state = SlugState::SHIELDED;
                                break;
                            case FoodType::POOP:
                                slugs[i]->state_record.state = SlugState::STOPPED;
                                break;
                        }
                        foods[i] = nullptr;
                    }
                    // generate new food?
                    double food_appear_probability = 1 * slugs[i]->year_boost;
                    if (slugs[i]->total_distance > food_appear_x && !foods[i] && rand() < food_appear_probability * RAND_MAX && end_banner.x > 250) {
                        std::cout << "food appeared at line " << i << std::endl;
                        int food_type = rand() % 3;
                        bool is_poop = rand() < (0.2 - food_appear_probability) * RAND_MAX;
                        if (!is_poop) foods[i] = std::make_unique<Food>(renderer, &display, static_cast<FoodType>(food_type), 300, slugs[i]->y);
                        else foods[i] = std::make_unique<Food>(renderer, &display, FoodType::POOP, 300, slugs[i]->y);
                    }
                }
            }

            // Check if any slug has reached the end
            for (int i = 0; i < line_count; i++) {
                if (slugs[i]->rank == -1 && slugs[i]->isBumped(&end_banner)) {
                    slugs[i]->rank = i;
                    slugs[i]->state_record.state = SlugState::MOVING;
                }
            }

            // Check if all slugs have reached the end
            bool all_reached = true;
            for (int i = 0; i < line_count; i++) {
                if (slugs[i]->rank == -1) {
                    all_reached = false;
                    break;
                }
            }

            if (all_reached) {
                all_slugs_reached = true;
                // running = false;
            }
        }

        // all slugs reached, end
        else {
            for (int i = 0; i < line_count; i++) {
                slugs[i]->x = end_banner.x;
            }
            display_all();
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

            end_banner.display();
            
            for (int i = 0; i < line_count; i++) {
                if (foods[i]) {
                    foods[i]->display();
                }
                slugs[i]->display();
            }

            if (game_time >= show_year_time && game_time < show_year_time + countdown_hold_time * 3) {
                display.display_countdown((game_time - show_year_time) / countdown_hold_time);
            }

            display.fill_bottom();
            SDL_RenderPresent(renderer);
            break;
    }
}
