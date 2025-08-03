#include <iostream>
#include "Sway.h"
#include <SDL3/SDL.h>
#include <fontconfig/fontconfig.h>
#include "ui/GUI.h"
#include "ui/FontManager.h"

/**
 *  GUI for toggling Sway outputs
 */
int main() {
    Sway sway_ipc;
    auto sway_outputs = sway_ipc.get_outputs();
    if (sway_outputs.empty()) {
        // Should never happen, unless Sway returns data in a different format
        std::cerr << "No outputs found" << std::endl;
        return 1;
    }
    const auto window_width = sway_outputs.size() * 250;
    constexpr auto window_height = 160;

    if (!SDL_Init(SDL_INIT_VIDEO)) {
        std::cerr << "Could not initialize SDL" << std::endl;
        return 1;
    }

    SDL_Window *window = nullptr;
    SDL_Renderer *renderer = nullptr;

    if (!SDL_CreateWindowAndRenderer("Sway Display Switch", window_width, window_height, SDL_WINDOW_BORDERLESS, &window,
                                     &renderer)) {
        std::cerr << "Could not create window and renderer" << std::endl;
        return 1;
    }


    ui::FontManager font_manager{};
    auto font_filepath = font_manager.get_font_filepath();
    ui::GUI gui(renderer, window, sway_outputs, font_filepath,
                [&sway_ipc](const std::string &name, bool state) {
                    std::cout << name << ": " << std::boolalpha << state << std::endl;
                    return sway_ipc.set_output(name, state);
                });

    bool exit = false;
    gui.draw();
    while (!exit) {
        SDL_Event e;
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_EVENT_QUIT)
                exit = true;

            if (e.type == SDL_EVENT_KEY_DOWN) {
                if (e.key.key == SDLK_ESCAPE || e.key.key == SDLK_Q) {
                    exit = true;
                    break;
                }
                gui.handle_input(e.key.key);
                gui.draw();
            }
        }

        SDL_Delay(10);
    }


    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
