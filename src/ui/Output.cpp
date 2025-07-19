#include "Output.h"

namespace ui {
    void Output::draw(float x, float y, float w, float h) const {
        if (sway_info.active) {
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            SDL_FRect ui_elem_rect{.x = x, .y = y, .w = w, .h = h};
            SDL_RenderFillRect(renderer, &ui_elem_rect);
        }

        SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);
        SDL_FRect text_rect(x, y + h / 2, texture->w, texture->h);
        SDL_RenderTexture(renderer, texture, nullptr, &text_rect);
    }
}
