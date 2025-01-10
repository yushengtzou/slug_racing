#include <SDL.h>
#include <iostream>
#include <cstdlib> // For rand()
#include <vector>

// Update these constants to be initialized later
const int SCREEN_WIDTH = 800;  // These values will be overwritten
const int SCREEN_HEIGHT = 600; // These values will be overwritten
const int SLUG_WIDTH = 60;
const int SLUG_HEIGHT = 40;

struct Slug {
    float x = 0;
    float speed = 0;
    SDL_Rect rect;
};

int main(int argc, char* argv[]) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "Failed to initialize SDL: " << SDL_GetError() << std::endl;
        return -1;
    }

    // Get the display mode to set proper fullscreen dimensions
    SDL_DisplayMode DM;
    SDL_GetCurrentDisplayMode(0, &DM);
    const int SCREEN_WIDTH = DM.w;
    const int SCREEN_HEIGHT = DM.h;

    SDL_Window* window = SDL_CreateWindow(
        "Lottery Game",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        SCREEN_WIDTH, SCREEN_HEIGHT,
        SDL_WINDOW_SHOWN | SDL_WINDOW_FULLSCREEN  // Add fullscreen flag
    );

    if (!window) {
        std::cerr << "Failed to create window: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return -1;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        std::cerr << "Failed to create renderer: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        SDL_Quit();
        return -1;
    }

    // Initialize two slugs
    std::vector<Slug> slugs(2);
    for (int i = 0; i < slugs.size(); i++) {
        slugs[i].rect.x = 0;
        slugs[i].rect.y = 150 + i * 150;
        slugs[i].rect.w = SLUG_WIDTH;
        slugs[i].rect.h = SLUG_HEIGHT;
    }

    bool running = true;
    bool raceStarted = false;
    SDL_Event event;

    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            }
            if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE) {
                running = false;
            }
            if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_SPACE && !raceStarted) {
                raceStarted = true;
                // Set random speeds for slugs
                for (auto& slug : slugs) {
                    slug.speed = 1.0f + (rand() % 100) / 100.0f; // Speed between 1.0 and 2.0
                }
            }
        }

        // Update slug positions
        if (raceStarted) {
            bool raceFinished = false;
            for (auto& slug : slugs) {
                slug.x += slug.speed;
                slug.rect.x = (int)slug.x;
                
                if (slug.rect.x >= SCREEN_WIDTH - SLUG_WIDTH) {
                    raceFinished = true;
                }
            }

            if (raceFinished) {
                // Find winner
                int winner = (slugs[0].x > slugs[1].x) ? 1 : 2;
                std::cout << "Slug " << winner << " wins!" << std::endl;
                raceStarted = false;
                // Reset positions
                for (auto& slug : slugs) {
                    slug.x = 0;
                    slug.rect.x = 0;
                }
            }
        }

        // Render
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        // Draw finish line
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        SDL_Rect finishLine = {SCREEN_WIDTH - 10, 0, 10, SCREEN_HEIGHT};
        SDL_RenderFillRect(renderer, &finishLine);

        // Draw slugs
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        for (const auto& slug : slugs) {
            SDL_RenderFillRect(renderer, &slug.rect);
        }

        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
