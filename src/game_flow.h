#pragma once
#include <SDL2/SDL.h>
#include "display.h"
#include "game_state.h"
#include "slug_enum.h"

struct Slug {
    SlugType type;
    SlugState state;
    int x;
    int y;
    int frame;
};

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
    Uint32 game_time;

    int slug_count;
    Slug slugs[5];

    void calc_frame(Slug& slug);
};