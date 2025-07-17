#include <iostream>
#include "Sway.h"
#include <SDL3/SDL.h>

int main() {
    constexpr auto window_width = 500;
    constexpr auto window_height = 200;

    Sway sway_ipc;
    auto sway_outputs = sway_ipc.get_outputs();
    for (const auto &output: sway_outputs) {
        std::cout << "output: " << output.name << ": " << std::boolalpha << output.active << std::endl;
    }
    if (sway_outputs.empty()) {
        // Should never happen, unless Sway returns data in a different format
        std::cerr << "No outputs found" << std::endl;
        return 1;
    }

    const float ui_elem_len = window_width / sway_outputs.size();


    if (!SDL_Init(SDL_INIT_VIDEO)) {
        std::cerr << "Could not initialize SDL" << std::endl;
        return 1;
    }

    SDL_Window *window;
    SDL_Renderer *renderer;

    if (!SDL_CreateWindowAndRenderer("Sway Monitor Switch", window_width, window_height, SDL_WINDOW_BORDERLESS, &window,
                                     &renderer)) {
        std::cerr << "Could not create window and renderer" << std::endl;
        return 1;
    }

    auto selected_output = 0;

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
                        if (selected_output == 0)
                            selected_output = sway_outputs.size() - 1;
                        else
                            selected_output--;
                        break;
                    }
                    case SDLK_RIGHT: {
                        std::cout << "right" << std::endl;
                        if (selected_output == sway_outputs.size() - 1)
                            selected_output = 0;
                        else
                            selected_output++;
                        break;
                    }
                    case SDLK_SPACE:
                    case SDLK_RETURN: {
                        auto &output = sway_outputs.at(selected_output);
                        output.active = !output.active;
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

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);


        for (auto i = 0; const auto &output: sway_outputs) {
            if (output.active) {
                SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
                SDL_FRect ui_elem_rect{.x = 0 + i * ui_elem_len, .y = 0, .w = ui_elem_len, .h = window_height};
                SDL_RenderFillRect(renderer, &ui_elem_rect);
            }

            i++;
        }

        SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);
        auto selected_x = selected_output * ui_elem_len + 10;
        SDL_FRect selected_rect{.x = selected_x, .y = 10, .w = 10, .h = 10};
        SDL_RenderFillRect(renderer, &selected_rect);


        SDL_RenderPresent(renderer);
        SDL_Delay(10);
    }


    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
