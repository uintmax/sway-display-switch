#include <iostream>
#include "Sway.h"
#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <fontconfig/fontconfig.h>
#include "ui/GUI.h"

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

    if (!FcInit()) {
        std::cerr << "Could not initialize fontconfig" << std::endl;
        return 1;
    }

    // TODO: Add error messages
    FcPattern *font_pattern = FcNameParse(reinterpret_cast<const FcChar8 *>(":Mono"));
    if (!font_pattern) {
        return 1;
    }

    if (!FcConfigSubstitute(nullptr, font_pattern, FcMatchPattern)) {
        return 1;
    }
    FcDefaultSubstitute(font_pattern);

    FcResult font_result;
    FcPattern *font_match = FcFontMatch(nullptr, font_pattern, &font_result);
    if (!font_match || font_result != FcResultMatch) {
        return 1;
    }
    FcChar8 *font_file_path = nullptr;
    if (FcPatternGetString(font_match, FC_FILE, 0, &font_file_path) != FcResultMatch) {
        return 1;
    }

    std::printf("%s\n", font_file_path);
    // TODO: Free fontconfig resources
    // TODO: Move font initialization to extra class

    if (!TTF_Init()) {
        std::cerr << "Could not initialize TTF renderer" << std::endl;
        return 1;
    }

    TTF_Font *font = TTF_OpenFont(reinterpret_cast<char *>(font_file_path), 20);
    if (!font) {
        std::cerr << "Could not find font" << std::endl;
        return 1;
    }

    ui::GUI gui(renderer, window, sway_outputs, font,
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
                if (e.key.key == SDLK_ESCAPE) {
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
