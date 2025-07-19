#ifndef OUTPUT_H
#define OUTPUT_H
#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include "../Sway.h"
#include <stdexcept>

namespace ui {
    // TODO: Free resources in deconstructor
    class Output {
    public:
        Output(const SwayOutput &sway_output, SDL_Renderer *renderer, TTF_Font *font): sway_info(sway_output),
            renderer(renderer), font(font) {
            SDL_Color font_color{100, 100, 100, 255};
            surface = TTF_RenderText_Blended(font, sway_output.name.c_str(), 0, font_color);
            if (!surface)
                throw std::runtime_error("todo");
            texture = SDL_CreateTextureFromSurface(renderer, surface);
            if (!texture)
                throw std::runtime_error("todo");
        }

        void draw(float x, float y, float w, float h) const;

        SwayOutput sway_info;

    private:
        SDL_Renderer *renderer;
        TTF_Font *font;
        SDL_Surface *surface = nullptr;
        SDL_Texture *texture = nullptr;
    };
}


#endif //OUTPUT_H
