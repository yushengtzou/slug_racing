#include "game_flow.h"
#include <SDL2/SDL.h>
#include <iostream>

const int GameFlow::start_x = 50;

const int FRAME_DELAY = 150;

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
    fastest_x = 120;
    all_slugs_reached = false;
    any_slug_reached = false;
    int start_y = 18;
    int interval_y = 20;

    narration_count = 0;
    for(int i = 0; i < narration_type_count; i++) {
        narration_appeared[i] = false;
        narration_pos[i] = 0;
    }

    for(int i = 0; i < 50; i++) {
        last_fastest_indexes[i] = -1;
    }
    num_of_slugs_reached = 0;
    switch (state) {
        case EmployeeGame:
            line_count = 5;
            slugs[0] = std::make_unique<Slug>(renderer, &display, display.get_slug_texture(AI_Wes), display.get_effect_texture(), display.zoom_factor, AI_Wes, GameFlow::start_x, start_y, 2.0);
            slugs[1] = std::make_unique<Slug>(renderer, &display, display.get_slug_texture(DS), display.get_effect_texture(), display.zoom_factor, DS, GameFlow::start_x, start_y + interval_y, 2.0);
            slugs[2] = std::make_unique<Slug>(renderer, &display, display.get_slug_texture(MM_YuMing), display.get_effect_texture(), display.zoom_factor, MM_YuMing, GameFlow::start_x, start_y + interval_y * 2, 1.0);
            slugs[3] = std::make_unique<Slug>(renderer, &display, display.get_slug_texture(MM_MuHuan), display.get_effect_texture(), display.zoom_factor, MM_MuHuan, GameFlow::start_x, start_y + interval_y * 3, 1.0);
            slugs[4] = std::make_unique<Slug>(renderer, &display, display.get_slug_texture(MM_Lulu), display.get_effect_texture(), display.zoom_factor, MM_Lulu, GameFlow::start_x, start_y + interval_y * 4, 3.0);
            food_appear_x = 50;
            end_banner_appear_x = employee_map_length;
            end_banner.x += end_banner_appear_x;
            show_year_time = 16;
            break;
        case InternGame:
            line_count = 2;
            slugs[0] = std::make_unique<Slug>(renderer, &display, display.get_slug_texture(AI_Daniel), display.get_effect_texture(), display.zoom_factor, AI_Daniel, GameFlow::start_x, 50, 1.0);
            slugs[1] = std::make_unique<Slug>(renderer, &display, display.get_slug_texture(AI_Benson), display.get_effect_texture(), display.zoom_factor, AI_Benson, GameFlow::start_x, 90, 1.0);
            food_appear_x = 100;
            end_banner_appear_x = 2000;
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

        // std::cout << "game_time: " << game_time << std::endl;
        display_all();

        if (game_time < begin_hold_time + show_year_time + countdown_hold_time * 3) {
            if (game_time == 0) add_narration("記得為自己喜歡的選手加油哇，語音辨識模型當時fine tune了五天QQ", NarrationType::AllStart);
            else if (game_time == begin_hold_time - 6) add_narration("老鳥福利，Lulu和Tako獲得年資加成效果:3", NarrationType::IntroStart);
            
            for (int i = 0; i < line_count; i++) {
                slugs[i]->state_record.frame = 0;
            }
        }

        // game logic
        else if (!all_slugs_reached) {if(game_time == begin_hold_time + show_year_time + countdown_hold_time * 3) add_narration("比賽開始！", NarrationType::GameStart);
            // Check if any slug has reached the end
            for (int i = 0; i < line_count; i++) {
                if (slugs[i]->rank == -1 && (slugs[i]->isBumped(&end_banner) || slugs[i]->x > end_banner.x)) {
                    num_of_slugs_reached++;
                    slugs[i]->rank = num_of_slugs_reached;
                    slugs[i]->state_record = SlugStateRecord(SlugState::MOVING);
                    slugs[i]->x = end_banner.x;
                    if(state == EmployeeGame){
                        switch(num_of_slugs_reached) {
                            case 1:
                                add_narration("冠軍出爐！由快跑王者" + slugs[i]->name + "奪得三萬獎金！", NarrationType::Champion);
                                break;
                            case 2:
                                add_narration("亞軍由" + slugs[i]->name + "獲得，喜提兩萬元！", NarrationType::Second);
                                break;
                            case 3:
                                // add_narration("季軍的" + slugs[i]->name + "，準備開開心心帶一萬元回家！", NarrationType::Third);
                                break;
                            case 4:
                                // add_narration("殿軍是" + slugs[i]->name + "，得到六千元！", NarrationType::Fourth);
                                break;
                            case 5:
                                // add_narration("" + slugs[i]->name + "努力完賽的精神值得敬佩，獎金六千元！", NarrationType::Fifth);
                                break;
                        }
                    }
                    any_slug_reached = true;
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

            // Calculate speeds and find fastest slug
            int fastest_slug_index = 0;
            double year_boost_factor = .17;
            double food_boost_factor = .8;
            for (int i = 0; i < line_count; i++) {
                if (slugs[i]->rank == -1) {
                    const double max_random = 4.0;
                    if(slugs[i]->state_record.state == SlugState::MOVING || slugs[i]->state_record.state == SlugState::BOOSTED || slugs[i]->state_record.state == SlugState::SUPER_BOOSTED || slugs[i]->state_record.state == SlugState::SHIELDED) {
                        double random_factor = static_cast<double>(rand()) / RAND_MAX * max_random;
                        double speed_base = slugs[i]->x < 60 ? 2.5 : 1.0;
                        if(slugs[i]->x < 30) speed_base = 4.6;
                        if(any_slug_reached) speed_base = std::max(3.5, speed_base);
                        slugs[i]->speed = static_cast<int>(( 1.0 + slugs[i]->year_boost * year_boost_factor + 
                                                                slugs[i]->food_boost * food_boost_factor) * 
                                                        random_factor * speed_base);
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

            // sort slugs by total_distance, update ingame_rank
            for(int i = 0; i < line_count; i++) {
                slugs[i]->ingame_rank = 0;
            }
            for(int i = 0; i < line_count; i++) {
                for(int j = 0; j < line_count; j++) {
                    if(i != j && slugs[j]->total_distance >= slugs[i]->total_distance) {
                        slugs[i]->ingame_rank++;
                    }
                }
            }

            // narration
            if (end_banner.x > 300) {
                int back_track_count = 3;
                bool steady_win = last_fastest_indexes[2 * back_track_count] != -1 && last_fastest_indexes[2 * back_track_count] != fastest_slug_index;
                for(int j = back_track_count; j < 2 * back_track_count; j++) {
                    if(last_fastest_indexes[j] == -1 || last_fastest_indexes[j+1] == -1 || last_fastest_indexes[j] != last_fastest_indexes[j+1]) {
                        steady_win = false;
                        break;
                    }
                }
                for(int j = 0; j < back_track_count; j++) {
                    if(last_fastest_indexes[j] == -1 || last_fastest_indexes[j] != fastest_slug_index) {
                        steady_win = false;
                        break;
                    }
                }
                if (steady_win) {
                    if(!narration_appeared[NarrationType::Overtake]) add_narration(slugs[fastest_slug_index]->name + "一個閃身迅速超越" + slugs[last_fastest_indexes[back_track_count]]->name + "，毫不留情！", NarrationType::Overtake);
                    else if (!narration_appeared[NarrationType::Overtake_2]) add_narration("冠軍寶座再度易主，" + slugs[last_fastest_indexes[back_track_count]]->name + "慘遭" + slugs[fastest_slug_index]->name + "直道超車！", NarrationType::Overtake_2);
                    else if (!narration_appeared[NarrationType::Overtake_3]) add_narration("ATOP尾牙再傳超車！" + slugs[fastest_slug_index]->name + "笑：準備多時", NarrationType::Overtake_3);
                    else if (!narration_appeared[NarrationType::Overtake_4]) add_narration(slugs[fastest_slug_index]->name + "又超" + slugs[last_fastest_indexes[back_track_count]]->name + "！究竟是兔性的扭曲，還是道德的淪喪？", NarrationType::Overtake_4);
                    else if (!narration_appeared[NarrationType::Overtake_5]) add_narration("海兔們競爭激烈，" + slugs[fastest_slug_index]->name + "的新王座又能維持多久？", NarrationType::Overtake_5);
                }

                int see_only_one_winner_distance = 20;
                bool always_win = true;
                for(int i = 0; i < line_count; i++) {
                    if(i == fastest_slug_index) continue;
                    if(slugs[fastest_slug_index]->total_distance - slugs[i]->total_distance < see_only_one_winner_distance) {
                        always_win = false;
                        break;
                    }
                }
                if(always_win) {
                    if(!narration_appeared[NarrationType::AlwaysWin]) add_narration(slugs[fastest_slug_index]->name + "快得像一陣風", NarrationType::AlwaysWin);
                    else if(!narration_appeared[NarrationType::AlwaysWin_2]) add_narration("" + slugs[fastest_slug_index]->name + "怎麼獨霸了？其他親友快喊喊><", NarrationType::AlwaysWin_2);
                    else if(!narration_appeared[NarrationType::AlwaysWin_3]) add_narration("翻翻黃曆，今天" + slugs[fastest_slug_index]->name + "宜冠軍<3", NarrationType::AlwaysWin_3);
                    else if(!narration_appeared[NarrationType::AlwaysWin_4]) add_narration(slugs[fastest_slug_index]->name + "動如脫兔，有霸主之姿！", NarrationType::AlwaysWin_4); 
                    else if(!narration_appeared[NarrationType::AlwaysWin_5]) add_narration(slugs[fastest_slug_index]->name + "分享當No.1秘訣：不喜歡當No.兔", NarrationType::AlwaysWin_5);
                }

                bool has_win_in_interval[5] = {false};
                int winners_in_interval = 0;
                int change_interval = 6;
                for(int j = 0; j < line_count; j++) {
                    for(int k = 0; k < change_interval; k++) {
                        if(last_fastest_indexes[k] == j) {
                            if(!has_win_in_interval[j]) {
                                winners_in_interval++;
                                has_win_in_interval[j] = true;
                            }
                        }
                    }
                }
                if(winners_in_interval >= 3) {
                    if(!narration_appeared[NarrationType::ChangingChampion]) {
                        add_narration("目前選手們比數十分接近，冠軍也不斷變化！", NarrationType::ChangingChampion);
                    }
                }
                for(int j = 49; j >= 1; j--) {
                    last_fastest_indexes[j] = last_fastest_indexes[j - 1];
                }
                last_fastest_indexes[0] = fastest_slug_index;

                // add something to narration
                if(narration_count == 0){
                    int random_slug_index = rand() % line_count;
                    if(!narration_appeared[NarrationType::ShittyTalk]) add_narration(slugs[random_slug_index]->name + "人緣真好，大家都幫他加油", NarrationType::ShittyTalk);
                    else if(!narration_appeared[NarrationType::ShittyTalk_2]) add_narration("幫" + slugs[random_slug_index]->name + "加油的人也太大聲了吧，主持人檢查一下有沒有偷帶喇叭？", NarrationType::ShittyTalk_2);
                    else if(!narration_appeared[NarrationType::ShittyTalk_3]) add_narration(slugs[random_slug_index]->name + "眼裡透著三分薄涼、三分似笑非笑、四分漫不經心", NarrationType::ShittyTalk_3);
                    else if(!narration_appeared[NarrationType::ShittyTalk_4]) add_narration(slugs[random_slug_index]->name + " 言兌 言舌 女子 月半", NarrationType::ShittyTalk_4);
                    else if(!narration_appeared[NarrationType::ShittyTalk_5]) add_narration("哦！戰況非常激烈非常激烈！好興奮！", NarrationType::ShittyTalk_5);
                    else if(!narration_appeared[NarrationType::ShittyTalk_6]) add_narration(slugs[random_slug_index]->name + "的加油變小聲了，三秒內不加大速度就往下囉", NarrationType::ShittyTalk_6);
                    else if(!narration_appeared[NarrationType::ShittyTalk_7]) add_narration("海兔們努力扭動著，你怎麼還在看字幕！", NarrationType::ShittyTalk_7);
                    else if(!narration_appeared[NarrationType::ShittyTalk_8]) add_narration(slugs[random_slug_index]->name + "刷新了音量紀錄，達到驚人的 85.3 分貝！", NarrationType::ShittyTalk_8);
                    else if(!narration_appeared[NarrationType::ShittyTalk_9]) add_narration("（詞窮）（乾咳）加油加油！", NarrationType::ShittyTalk_9);
                    else if(!narration_appeared[NarrationType::ShittyTalk_10]) add_narration("快結束了！大聲喊出你支持的選手！", NarrationType::ShittyTalk_10);
                }
            }

            // calc cam movement when no slug has reached the end
            if(!any_slug_reached) cam_move_x = slugs[fastest_slug_index]->speed;
            // calc cam movement when some slug has reached the end
            else {
                int end_banner_x_after_someone_reached = 160;
                int speed_to_reach_end_banner_x_after_someone_reached = 10;
                if(end_banner.x + speed_to_reach_end_banner_x_after_someone_reached < end_banner_x_after_someone_reached) {
                    cam_move_x = - speed_to_reach_end_banner_x_after_someone_reached;
                }
                else if (end_banner.x < end_banner_x_after_someone_reached) {
                    cam_move_x = - end_banner_x_after_someone_reached + end_banner.x;
                }
                else cam_move_x = 0;
            }
            
            // Move background with camera
            display.add_background_offset(cam_move_x);
            // Move foods with camera
            for (int i = 0; i < line_count; i++) {
                if (foods[i]) {
                    foods[i]->move_left(cam_move_x);
                }
            }
            end_banner.move_left(cam_move_x);

            // Move slugs while no slug has reached the end
            if (!any_slug_reached) {
                // before a slug reaches fastest_x
                if (slugs[fastest_slug_index]->total_distance < GameFlow::fastest_x - GameFlow::start_x) {
                    for (int i = 0; i < line_count; i++) {
                        slugs[i]->x = (static_cast<int>(slugs[i]->x + slugs[i]->speed));
                    }
                }
                // after a slug reaches fastest_x
                else {
                    for (int i = 0; i < line_count; i++) {
                        if (i == fastest_slug_index) {
                            slugs[i]->x = GameFlow::fastest_x;
                        }
                        else {
                            slugs[i]->x = (static_cast<int>(GameFlow::fastest_x - slugs[fastest_slug_index]->total_distance + slugs[i]->total_distance));
                        }
                    }
                }
            }
            // Move slugs when some slug has reached the end
            else {
                for (int i = 0; i < line_count; i++) {
                    if (slugs[i]->rank == -1) {
                        slugs[i]->x = std::min(end_banner.x, slugs[i]->x + slugs[i]->speed);
                    }
                    else slugs[i]->x = end_banner.x;
                }
            }

            // for slugs that are far away, make them closer
            int nearest_slug_far_away_from_end = -1;
            for (int i = 0; i < line_count; i++) {
                if (slugs[i]->rank == -1 && slugs[i]->x < -50) {
                    if (nearest_slug_far_away_from_end == -1 || slugs[i]->total_distance < slugs[nearest_slug_far_away_from_end]->total_distance) {
                        nearest_slug_far_away_from_end = i;
                    }
                }
            }
            if (nearest_slug_far_away_from_end != -1) {
                std::cout << "moved nearest slug far away, " << nearest_slug_far_away_from_end << ", from " << slugs[nearest_slug_far_away_from_end]->x << ", to " << std::max(slugs[nearest_slug_far_away_from_end]->x, -50) << ", movement: " << std::max(slugs[nearest_slug_far_away_from_end]->x, -50) - slugs[nearest_slug_far_away_from_end]->x << std::endl;
                int movement = std::max(slugs[nearest_slug_far_away_from_end]->x, -50) - slugs[nearest_slug_far_away_from_end]->x;
                slugs[nearest_slug_far_away_from_end]->x += movement;
                slugs[nearest_slug_far_away_from_end]->total_distance += movement;
            }


            for (int i = 0; i < line_count; i++) {
                // frame control
                slugs[i]->state_record.frame = (slugs[i]->state_record.frame + 1) % slugs[i]->state_record.frameCount;

                // event animation (e.g. boosted) time control
                if (slugs[i]->state_record.animation_duration != -1) {
                    slugs[i]->state_record.animation_time++;
                    if (slugs[i]->state_record.animation_time >= slugs[i]->state_record.animation_duration) {
                        std::cout << slugs[i]->state_record.state << " effect of slug " << i << " ended" << std::endl;
                        slugs[i]->change_speed_factors("return", 1.0);
                        slugs[i]->state_record = SlugStateRecord(SlugState::MOVING);
                    }
                }

                if (slugs[i]->state_record.state == SlugState::MOVING) {
                    // bump into food?
                    // if(foods[i]) std::cout << "food position: " << foods[i]->x << " slug position: " << slugs[i]->x << " food type: " << foods[i]->food_type << " slug state: " << slugs[i]->state_record.state << std::endl;
                    if (foods[i] && slugs[i]->isBumped(foods[i].get())) {
                        std::cout << "slug " << i << " bumped into food " << foods[i]->food_type << std::endl;
                        switch (foods[i]->food_type) {
                            case FoodType::RED:
                                slugs[i]->state_record = SlugStateRecord(SlugState::SUPER_BOOSTED);
                                slugs[i]->change_speed_factors("food", 2.8);
                                break;
                            case FoodType::GREEN:
                                slugs[i]->state_record = SlugStateRecord(SlugState::BOOSTED);
                                slugs[i]->change_speed_factors("food", 1.8);
                                break;
                            case FoodType::BLUE:
                                slugs[i]->state_record = SlugStateRecord(SlugState::SHIELDED);
                                slugs[i]->change_speed_factors("food", 1.2);
                                break;
                            case FoodType::POOP:
                                slugs[i]->state_record = SlugStateRecord(SlugState::STOPPED);
                                slugs[i]->change_speed_factors("food", 0.0);
                                break;
                        }
                        foods[i] = nullptr;
                    }
                    // generate new food?
                    std::string food_type_string[4] = {"RED", "GREEN", "BLUE", "POOP"};
                    int food_count = 0;
                    for(int i = 0; i < line_count; i++) {
                        if(foods[i]) food_count++;
                    }
                    double food_appear_probability = .001 * (5 - food_count) * (5 - food_count) * (5 - food_count);
                    int mid = 6;
                    // good ratio: mid-1, mid, mid+1; bad ratio: mid+1, mid, mid-1
                    bool good_food_appeared = rand() < food_appear_probability / (mid + 1) * (mid - 2 + slugs[i]->year_boost) * RAND_MAX;
                    bool poop_appeared = rand() < food_appear_probability * .2 / (mid + 1) * (mid + 2 - slugs[i]->year_boost) * RAND_MAX;
                    if (slugs[i]->total_distance > food_appear_x && !foods[i] && slugs[i]->state_record.state == SlugState::MOVING && end_banner.x > 350) {
                        if(good_food_appeared) {
                            int food_type = rand() % 3;
                            std::cout << "food " << food_type_string[food_type] << " appeared at line " << i << std::endl;
                            foods[i] = std::make_unique<Food>(renderer, &display, static_cast<FoodType>(food_type), 300, slugs[i]->y);
                        }
                        else if (poop_appeared) {
                            std::cout << "poop appeared at line " << i << std::endl;
                            foods[i] = std::make_unique<Food>(renderer, &display, FoodType::POOP, 300, slugs[i]->y);
                        }
                    }
                }
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

            int text_year_offset_x = 13;
            int text_year_offset_y = 5;
            int blank = 2;
            if (game_time >= begin_hold_time && game_time < begin_hold_time + show_year_time / 2 - blank) {
                display.display_text("年資加成2點", slugs[1]->x + text_year_offset_x, slugs[1]->y - text_year_offset_y - game_time + begin_hold_time, 16, SDL_Color{0, 0, 0}, (int)(255 - 255 * (game_time - begin_hold_time) / (show_year_time / 2 - blank)));
            }
            else if (game_time >= begin_hold_time + show_year_time / 2 && game_time < begin_hold_time + show_year_time - blank) {
                display.display_text("年資加成3點", slugs[4]->x + text_year_offset_x, slugs[4]->y - text_year_offset_y - game_time + begin_hold_time + show_year_time / 2, 16, SDL_Color{0, 0, 0}, (int)(255 - 255 * (game_time - begin_hold_time - show_year_time / 2) / (show_year_time / 2 - blank)));
            }

            else if (game_time < begin_hold_time + show_year_time + countdown_hold_time * 3) {
                display.display_countdown((game_time - begin_hold_time - show_year_time) / countdown_hold_time);
            }

            display_narration();

            display.fill_bottom();
            SDL_RenderPresent(renderer);
            break;
    }
}

void GameFlow::add_narration(std::string narration, NarrationType type) {
    int max_total_distance = 0;
    for(int i = 0; i < line_count; i++) {
        if(slugs[i]->total_distance > max_total_distance) max_total_distance = slugs[i]->total_distance;
    }
    if(max_total_distance < narration_constrants[type][0] || max_total_distance > narration_constrants[type][1]) return;

    if(narration_count >= narration_count_max) {
        // overwrite the oldest narration
        for(int i = 0; i < narration_count - 1; i++) {
            narration[i] = narration[i + 1];
            narration_time[i] = narration_time[i + 1];
            narration_types[i] = narration_types[i + 1];
            // narration_pos[i] = narration_pos[i + 1];
            narration_count--;
        }
    }
    this->narration[narration_count] = narration;
    this->narration_time[narration_count] = 0;
    this->narration_types[narration_count] = type;
    narration_count++;
    // this->narration_pos[narration_count] = this->narration_pos[narration_count - 1] + 1;
    narration_appeared[type] = true;
}

void GameFlow::display_narration() {
    if(state == InternGame) return;
    for(int i = 0; i < narration_count; i++) {
        display.display_text(narration[i], 30, 130 - i * 10, 12, SDL_Color{0, 0, 0}, 255 - std::max(0, narration_time[i] - narration_duration + narration_fade_duration) * 255 / narration_fade_duration);
        narration_time[i]++;
    }
    for(int i = 0; i < narration_count; i++) {
        if(narration_time[i] >= narration_duration) {
            narration_count--;
            for(int j = i; j < narration_count; j++) {
                narration[j] = narration[j + 1];
                narration_time[j] = narration_time[j + 1];
                // narration_pos[j] = narration_pos[j + 1];
            }
        }
    }
}
