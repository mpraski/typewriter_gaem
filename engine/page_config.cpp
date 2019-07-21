//
// Created by marcin on 7/21/19.
//

#include "page_config.h"

engine::page_config::page_config(
    const std::string &font_path,
    unsigned font_size,
    float page_width,
    float page_height,
    float margin_vertical,
    float margin_horizontal,
    float letter_spacing_factor,
    float line_spacing_factor,
    unsigned typing_delay)
    : font{new sf::Font},
      font_size{font_size},
      page_width{page_width},
      page_height{page_height},
      margin_vertical{margin_vertical},
      margin_horizontal{margin_horizontal},
      letter_spacing_factor{letter_spacing_factor},
      line_spacing_factor{line_spacing_factor},
      typing_delay{typing_delay * 100000u} {
  if (!font->loadFromFile(font_path)) {
    throw std::runtime_error("Unable to load font from path: " + font_path);
  }
}

engine::page_config::~page_config() {
  delete font;
}