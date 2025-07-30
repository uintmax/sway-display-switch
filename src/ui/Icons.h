#ifndef ICONS_H
#define ICONS_H
#include <string>

// Icons from Google: https://fonts.google.com/icons
namespace ui::icons {
    constexpr std::string_view laptop =
            "<svg xmlns='http://www.w3.org/2000/svg' height='48px' viewBox='0 -960 960 960' width='48px' fill='#1f1f1f'><path d='M0-160v-60h80v-620h800v620h80v60H0Zm396-60h170v-42H396v42ZM144-322h680v-458H144v458Zm340-229Z'/></svg>";
    constexpr std::string_view monitor =
            "<svg xmlns='http://www.w3.org/2000/svg' height='48px' viewBox='0 -960 960 960' width='48px' fill='#1f1f1f'><path d='M260-120v-73l47-47H140q-24 0-42-18t-18-42v-480q0-24 18-42t42-18h680q24 0 42 18t18 42v480q0 24-18 42t-42 18H652l48 47v73H260ZM140-300h680v-480H140v480Zm0 0v-480 480Z'/></svg>";
}
#endif //ICONS_H
