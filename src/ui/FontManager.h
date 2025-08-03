#ifndef FONTMANAGER_H
#define FONTMANAGER_H
#include <fontconfig/fontconfig.h>
#include <stdexcept>
#include <SDL3_ttf/SDL_ttf.h>
#include <string>


namespace ui {
    /**
     * Finds filepath of a font that matches the given pattern
     */
    class FontManager {
    public:
        FontManager(const std::string &pattern = ":Mono");

        std::string get_font_filepath() const;

    private:
        std::string font_filepath;
    };
}

#endif //FONTMANAGER_H
