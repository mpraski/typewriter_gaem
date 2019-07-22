//
// Created by marcin on 7/21/19.
//

#include "page_config.h"

engine::page_config::page_config(
    const std::string &font_path,
    const std::string &sounds_path,
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
      typing_delay{typing_delay * 100000u},
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
  if (!font->loadFromFile(font_path)) {
    throw std::runtime_error("Unable to load font from path: " + font_path);
  }
}

engine::page_config::~page_config() {
  delete font;
}

std::string engine::page_config::get_sub_directory(const fs::path &path) const {
  constexpr const auto separator{fs::path::preferred_separator};

  std::istringstream iss(path.string());
  std::vector<std::string> parts(std::istream_iterator<std::string>{iss},
                                 std::istream_iterator<std::string>());

  if (parts.empty()) {
    throw std::runtime_error("Provided path does not contain sub directory: " + path.string());
  }

  return parts[parts.size() - 1];
}