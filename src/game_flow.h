#pragma once
#include <SDL2/SDL.h>
#include "display.h"
#include "element.h"
#include "end.h"
#include "game_state.h"
#include "slug_enum.h"
#include "slug.h"
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

    static const int start_x, fastest_x;

    static const int MAX_LINES = 5;
    int line_count;
    std::array<std::unique_ptr<Slug>, MAX_LINES> slugs;
    std::array<std::deque<std::unique_ptr<Element>>, MAX_LINES> foods;

    End end_banner;

    int cam_move_x;
    void display_all();
};