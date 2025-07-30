#include "Output.h"

namespace ui {
    void Output::draw(float x, float y, float w, float h) const {
        if (sway_info.active) {
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            SDL_FRect ui_elem_rect{.x = x, .y = y, .w = w, .h = h};
            SDL_RenderFillRect(renderer, &ui_elem_rect);
        }

        SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);
        SDL_FRect text_rect(x + w / 2 - text_texture->w / 2, y + h / 2, text_texture->w, text_texture->h);
        SDL_RenderTexture(renderer, text_texture, nullptr, &text_rect);

        SDL_FRect icon_rect(x + w / 2 - icon_texture->w / 2, y + h / 4, icon_texture->w, icon_texture->h);
        SDL_RenderTexture(renderer, icon_texture, nullptr, &icon_rect);
    }
}
