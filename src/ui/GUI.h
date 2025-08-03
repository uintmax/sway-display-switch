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
    /**
     * GUI that shows available displays, handles input and executes given callback on selection
     */
    class GUI {
    public:
        GUI(SDL_Renderer *renderer, SDL_Window *window, const std::vector<SwayOutput> &sway_outputs,
            const std::string &font_filepath,
            const std::function<bool(const std::string &, bool)> &selection_callback): renderer(renderer),
            window(window),
            selection_callback(selection_callback) {
            SDL_GetWindowSize(window, &window_size.first, &window_size.second);
            ui_elem_len = window_size.first / sway_outputs.size();
            if (!TTF_Init())
                throw std::runtime_error("Could not initialize TTF renderer");

            font = TTF_OpenFont(font_filepath.c_str(), 20);
            if (!font)
                throw std::runtime_error("Could not find font");
            for (const auto &sway_output: sway_outputs) {
               output_elements.emplace_back(sway_output, renderer, font);
            }
        }

        ~GUI();

        void draw();

        void handle_input(SDL_Keycode key);

    private:
        SDL_Renderer *renderer = nullptr;
        SDL_Window *window = nullptr;
        TTF_Font *font = nullptr;
        std::pair<int, int> window_size;
        float ui_elem_len;
        std::vector<ui::Output> output_elements;
        std::function<bool(const std::string &, bool)> selection_callback;
        uint8_t selected_output = 0;
    };
}

#endif //GUI_H
