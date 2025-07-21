#include "GUI.h"

namespace ui {
    void GUI::draw() {
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);


        for (int i = 0; const auto &elem: output_elements) {
            elem.draw(ui_elem_len * i, 0, ui_elem_len, window_size.second);
            i++;
        }

        SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);
        auto selected_x = selected_output * ui_elem_len + 10;
        SDL_FRect selected_rect{.x = selected_x, .y = 10, .w = 10, .h = 10};
        SDL_RenderFillRect(renderer, &selected_rect);


        SDL_RenderPresent(renderer);
    }

    void GUI::handle_input(SDL_Keycode key) {
        switch (key) {
            case SDLK_A:
            case SDLK_H:
            case SDLK_LEFT: {
                std::cout << "left" << std::endl;
                if (selected_output == 0)
                    selected_output = output_elements.size() - 1;
                else
                    selected_output--;
                break;
            }
            case SDLK_D:
            case SDLK_L:
            case SDLK_RIGHT: {
                std::cout << "right" << std::endl;
                if (selected_output == output_elements.size() - 1)
                    selected_output = 0;
                else
                    selected_output++;
                break;
            }
            case SDLK_SPACE:
            case SDLK_RETURN: {
                auto &output = output_elements.at(selected_output);
                output.sway_info.active = !output.sway_info.active;
                if (!selection_callback(output.sway_info.name, output.sway_info.active))
                    throw std::runtime_error("Output state could not be changed");
                break;
            }
            default:
                break;
        }
    }
}
