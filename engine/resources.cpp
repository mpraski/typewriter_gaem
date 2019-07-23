//
// Created by marcin on 7/21/19.
//

#include "resources.h"

engine::resources::resources(
    const std::string &fonts_path,
    const std::string &sounds_path,
    unsigned font_size,
    float page_width,
    float page_height,
    float margin_vertical,
    float margin_horizontal,
    float letter_spacing_factor,
    float line_spacing_factor,
    unsigned typing_delay)
    : font{},
      font_size{font_size},
      page_width{page_width},
      page_height{page_height},
      margin_vertical{margin_vertical},
      margin_horizontal{margin_horizontal},
      letter_spacing_factor{letter_spacing_factor},
      line_spacing_factor{line_spacing_factor},
      typing_delay{typing_delay * 100000u},
      fonts{
          load_resources<sf::Font>(
              fonts_path,
              [](const auto &path) -> std::optional<sf::Font> {
                if (sf::Font f; f.loadFromFile(path)) {
                  return f;
                }
                return {};
              }
          )
      },
      sounds{
          load_resources<sf::SoundBuffer>(
              sounds_path,
              [](const auto &path) -> std::optional<sf::SoundBuffer> {
                if (sf::SoundBuffer buffer; buffer.loadFromFile(path)) {
                  return buffer;
                }
                return {};
              }
          )
      } {
  font = &fonts[ROOT_RESOURCE_CATEGORY][DEFAULT_FONT];
}