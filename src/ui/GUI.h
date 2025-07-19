#ifndef GUI_H
#define GUI_H

#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <vector>
#include "../Sway.h"
#include "Output.h"
#include <functional>
#include <string>
#include <stdexcept>
#include <fontconfig/fontconfig.h>

namespace ui {
    class GUI {
    public:
        GUI(SDL_Renderer *renderer, SDL_Window *window, const std::vector<SwayOutput> &sway_outputs, TTF_Font *font,
            const std::function<bool(const std::string &, bool)> &selection_callback): renderer(renderer),
            window(window),
            selection_callback(selection_callback) {
            SDL_GetWindowSize(window, &window_size.first, &window_size.second);
            ui_elem_len = window_size.first / sway_outputs.size();
            // TODO: Create font here
            for (const auto &sway_output: sway_outputs) {
                Output output_element(sway_output, renderer, font);
                output_elements.push_back(output_element);
            }
        }

        void draw();

        void handle_input(SDL_Keycode key);

    private:
        SDL_Renderer *renderer;
        SDL_Window *window;
        std::pair<int, int> window_size;
        float ui_elem_len;
        std::vector<ui::Output> output_elements;
        std::function<bool(const std::string &, bool)> selection_callback;
        uint8_t selected_output = 0;
    };
}

#endif //GUI_H
