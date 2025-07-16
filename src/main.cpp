#include <iostream>
#include "Sway.h"
#include <SDL3/SDL.h>

int main() {
    Sway sway_ipc;
    auto sway_outputs = sway_ipc.get_outputs();
    for (const auto &output: sway_outputs) {
        std::cout << "output: " << output.name << ": " << std::boolalpha << output.active << std::endl;
    }

    if (!SDL_Init(SDL_INIT_VIDEO)) {
        std::cerr << "Could not initialize SDL" << std::endl;
        return 1;
    }

    SDL_Window *window;
    SDL_Renderer *renderer;

    if (!SDL_CreateWindowAndRenderer("Sway Monitor Switch", 500, 200, SDL_WINDOW_BORDERLESS, &window, &renderer)) {
        std::cerr << "Could not create window and renderer" << std::endl;
        return 1;
    }


    bool exit = false;
    while (!exit) {
        SDL_Event e;
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_EVENT_QUIT)
                exit = true;

            if (e.type == SDL_EVENT_KEY_DOWN) {
                switch (e.key.key) {
                    case SDLK_LEFT: {
                        std::cout << "left" << std::endl;
                        break;
                    }
                    case SDLK_RIGHT: {
                        std::cout << "right" << std::endl;
                        break;
                    }
                    case SDLK_ESCAPE: {
                        exit = true;
                        break;
                    }
                    default:
                        break;
                }
            }
        }

        SDL_RenderClear(renderer);


        SDL_RenderPresent(renderer);
        SDL_Delay(10);
    }


    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
