#include <SDL2/SDL.h>
#include <iostream>
#include "display.h"
#include "game_flow.h"
#include "game_state.h"
#include "slug_enum.h"

#ifdef _WIN32
#undef main
#endif

const int SCREEN_WIDTH = 216;
const int SCREEN_HEIGHT = 144;

int main(int argc, char* argv[]) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "Failed to initialize SDL: " << SDL_GetError() << std::endl;
        return -1;
    }

    SDL_Window* window = SDL_CreateWindow(
        "ATOP Slug Racing",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        SCREEN_WIDTH, SCREEN_HEIGHT,
        SDL_WINDOW_SHOWN | SDL_WINDOW_FULLSCREEN_DESKTOP
    );

    if (!window) {
        std::cerr << "Failed to create window: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return -1;
    }

    int actual_width, actual_height;
    SDL_GetWindowSize(window, &actual_width, &actual_height);

    SDL_Renderer* sdl_renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!sdl_renderer) {
        std::cerr << "Failed to create renderer: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        SDL_Quit();
        return -1;
    }

    bool running = true;
    State state = InternGame;
    SDL_Event event;
    
    Uint32 frameStart;
    int frameTime;

    std::cout << "actual_width: " << actual_width << ", actual_height: " << actual_height << std::endl;
    Display display(sdl_renderer, actual_width, actual_height);

    Uint32 game_start = SDL_GetTicks();
    int game_time = 0;

    /*
    GameFlow game_flow(display, InternGame, sdl_renderer);
    game_flow.loop();
    */

    GameFlow game_flow(display, EmployeeGame, sdl_renderer);
    game_flow.loop();

    SDL_DestroyRenderer(sdl_renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
