#ifndef OUTPUT_H
#define OUTPUT_H
#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <SDL3_image/SDL_image.h>
#include <SDL3/SDL_iostream.h>
#include "../Sway.h"
#include <stdexcept>
#include "Icons.h"

namespace ui {
    /**
     * GUI element that represents a Sway output
     * Draws the output name, an icon and sets the background color depending on its state
     */
    class Output {
    public:
        Output(const SwayOutput &sway_output, SDL_Renderer *renderer, TTF_Font *font): sway_info(sway_output),
            renderer(renderer), font(font) {
            SDL_Color font_color{100, 100, 100, 255};
            text_surface = TTF_RenderText_Blended(font, sway_output.name.c_str(), 0, font_color);
            if (!text_surface)
                throw std::runtime_error("Could not render text");
            text_texture = SDL_CreateTextureFromSurface(renderer, text_surface);
            if (!text_texture)
                throw std::runtime_error("Could not create texture");
            is_embedded_display = sway_output.name.starts_with("eDP");

            auto icon = is_embedded_display ? icons::laptop : icons::monitor;
            auto io_stream = SDL_IOFromConstMem(icon.data(), icon.size());
            icon_texture = IMG_LoadTexture_IO(renderer, io_stream, true);
            if (!icon_texture)
                throw std::runtime_error("Could not load icon");
        }

        // TODO: Check out std::exchange
        Output(Output &&other) noexcept : sway_info(std::move(other.sway_info)), renderer(other.renderer),
                                          font(other.font),
                                          text_surface(other.text_surface),
                                          text_texture(other.text_texture),
                                          is_embedded_display(other.is_embedded_display),
                                          icon_texture(other.icon_texture) {
            other.renderer = nullptr;
            other.font = nullptr;
            other.text_surface = nullptr;
            other.text_texture = nullptr;
            other.icon_texture = nullptr;
        }

        ~Output();

        void draw(float x, float y, float w, float h) const;

        SwayOutput sway_info;

    private:
        SDL_Renderer *renderer = nullptr;
        TTF_Font *font = nullptr;
        SDL_Surface *text_surface = nullptr;
        SDL_Texture *text_texture = nullptr;
        bool is_embedded_display;
        SDL_Texture *icon_texture = nullptr;
    };
}


#endif //OUTPUT_H
