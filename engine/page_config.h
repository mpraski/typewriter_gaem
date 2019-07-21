//
// Created by marcin on 7/21/19.
//

#ifndef TYPEWRITER_GAEM_PAGE_CONFIG_H
#define TYPEWRITER_GAEM_PAGE_CONFIG_H

#include <memory>
#include "SFML/Graphics.hpp"

namespace engine {
class page_config {
public:
    page_config(
        const std::string &font_path,
        unsigned font_size = 24u,
        float page_width = 400.f,
        float page_height = 1000.f,
        float margin_vertical = 20.f,
        float margin_horizontal = 20.f,
        float letter_spacing_factor = 1.f,
        float line_spacing_factor = 1.f,
        unsigned typing_delay = 500u
    );

    ~page_config();

    page_config(const page_config &p) = delete;

    page_config(page_config &&p) = delete;

    page_config &operator=(const page_config &p) = delete;

    page_config &operator=(page_config &&p) = delete;

    sf::Font *font;
    unsigned int font_size;
    float page_width;
    float page_height;
    float margin_vertical;
    float margin_horizontal;
    float letter_spacing_factor;
    float line_spacing_factor;
    unsigned typing_delay;
};

using page_config_ptr = std::shared_ptr<const page_config>;

template<class ...Ts>
static auto make_page_config(Ts &&... args) {
  return std::make_shared<const page_config>(std::forward<Ts>(args)...);
}
}

#endif //TYPEWRITER_GAEM_PAGE_CONFIG_H
