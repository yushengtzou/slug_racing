#pragma once
#include <SDL2/SDL.h>
#include "display.h"
#include "element.h"
#include "end.h"
#include "game_state.h"
#include "slug_enum.h"
#include "slug.h"
#include "food.h"
#include <vector>
#include <deque>
#include <memory>
#include <array>
#include <cstdlib>
#include <ctime>

class GameFlow {
public:
    GameFlow(Display& display, State game_state, SDL_Renderer* renderer);
    ~GameFlow();

    void loop();

private:
    Display& display;
    SDL_Renderer* renderer;
    State state;
    bool running;
    long long game_time;

    static const int start_x;
    int fastest_x;
    double show_year_time;
    static const int countdown_hold_time = 4;
    static const int begin_hold_time = 20;
    int food_appear_x, end_banner_appear_x;

    bool all_slugs_reached, any_slug_reached;
    int num_of_slugs_reached;
    int last_fastest_indexes[50];

    static const int MAX_LINES = 5;
    int line_count;
    std::array<std::unique_ptr<Slug>, MAX_LINES> slugs;
    std::array<std::unique_ptr<Food>, MAX_LINES> foods;

    End end_banner;

    int cam_move_x;
    void display_all();

    static const int narration_duration = 30;
    static const int narration_fade_duration = 8;
    static const int narration_count_max = 3;
    int narration_count;
    std::string narration[narration_count_max];
    int narration_time[narration_count_max];
    int narration_pos[narration_count_max];
    static const int narration_type_count = 200;
    enum NarrationType {
        AllStart,
        IntroStart,
        GameStart,
        Champion,
        Overtake,
        Overtake_2,
        Overtake_3,
        Overtake_4,
        Overtake_5,
        AlwaysWin,
        AlwaysWin_2,
        AlwaysWin_3,
        AlwaysWin_4,
        AlwaysWin_5,
        ChangingChampion,
        Second,
        Third,
        Fourth,
        Fifth,
        ShittyTalk,
        ShittyTalk_2,
        ShittyTalk_3,
        ShittyTalk_4,
        ShittyTalk_5,
        ShittyTalk_6,
        ShittyTalk_7,
        ShittyTalk_8,
        ShittyTalk_9,
        ShittyTalk_10,
    };
    bool narration_appeared[narration_type_count];
    static const int employee_map_length = 1000;
    static const int inf = 10000000;
    static const int shitty_max = employee_map_length - 300;
    static const int shitty_min = 100, shitty_interval = (shitty_max - shitty_min) / 10;
    static const int overtake_interval = (employee_map_length - 400) / 5;
    static constexpr int narration_constrants[narration_type_count][2] = {
        // {min_distance, max_distance}
        {0, inf}, //AllStart
        {0, inf}, //IntroStart
        {0, inf}, //GameStart
        {0, inf}, //Champion
        {0, shitty_max}, //Overtake
        {overtake_interval, shitty_max}, //Overtake_2
        {overtake_interval * 2, shitty_max}, //Overtake_3
        {overtake_interval * 3, shitty_max}, //Overtake_4
        {overtake_interval * 4, shitty_max}, //Overtake_5
        {(int)(overtake_interval * 1.5), shitty_max}, //AlwaysWin
        {(int)(overtake_interval * 2.5), shitty_max}, //AlwaysWin_2
        {(int)(overtake_interval * 3.5), shitty_max}, //AlwaysWin_3
        {(int)(overtake_interval * 4.5), shitty_max}, //AlwaysWin_4
        {(int)(overtake_interval * 5.5), shitty_max}, //AlwaysWin_5
        {0, shitty_max}, //ChangingChampion
        {0, inf}, //Second
        {0, inf}, //Third
        {0, inf}, //Fourth
        {0, inf}, //Fifth
        {shitty_min, shitty_max}, //ShittyTalk
        {shitty_min + shitty_interval, shitty_max}, //ShittyTalk_2
        {shitty_min + shitty_interval * 2, shitty_max}, //ShittyTalk_3
        {shitty_min + shitty_interval * 3, shitty_max}, //ShittyTalk_4
        {shitty_min + shitty_interval * 4, shitty_max}, //ShittyTalk_5
        {shitty_min + shitty_interval * 5, shitty_max}, //ShittyTalk_6
        {shitty_min + shitty_interval * 6, shitty_max}, //ShittyTalk_7
        {shitty_min + shitty_interval * 7, shitty_max}, //ShittyTalk_8
        {shitty_min + shitty_interval * 8, shitty_max}, //ShittyTalk_9
        {shitty_min + shitty_interval * 9, shitty_max}, //ShittyTalk_10
    };
    NarrationType narration_types[narration_count_max];
    void add_narration(std::string narration, NarrationType type);
    void display_narration();
};