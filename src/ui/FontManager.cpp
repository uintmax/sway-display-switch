#include "FontManager.h"

namespace ui {
    FontManager::FontManager(const std::string &pattern) {
        if (!FcInit())
            throw std::runtime_error("Could not initialize fontconfig");

        FcPattern *font_pattern = FcNameParse(reinterpret_cast<const FcChar8 *>(pattern.c_str()));
        if (!font_pattern)
            throw std::runtime_error("Could not parse pattern string");

        if (!FcConfigSubstitute(nullptr, font_pattern, FcMatchPattern))
            throw std::runtime_error("Could not execute substitutions");

        FcDefaultSubstitute(font_pattern);

        FcResult font_result;
        FcPattern *font_match = FcFontMatch(nullptr, font_pattern, &font_result);
        if (!font_match || font_result != FcResultMatch)
            throw std::runtime_error("Could not find a font match");

        FcChar8 *filepath = nullptr;
        if (FcPatternGetString(font_match, FC_FILE, 0, &filepath) != FcResultMatch)
            throw std::runtime_error("Could not get font filepath");

        this->font_filepath = reinterpret_cast<char *>(filepath);
        FcPatternDestroy(font_match);


        // TODO: Free all fontconfig resources
    }

    std::string FontManager::get_font_filepath() const {
        return font_filepath;
    }
}
