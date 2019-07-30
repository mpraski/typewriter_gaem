//
// Created by marcin on 7/21/19.
//

#include "resources.h"

engine::resources::resources(
    sf::VideoMode mode,
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
    : mode{mode},
      curr_mouse{},
      prev_mouse{},
      cursor_type{sf::Cursor::Arrow},
      window{mode, "Title"},
      font{},
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

void engine::resources::display(const std::function<void(sf::RenderWindow &w)> &f) const {
  while (window.isOpen()) {
    sf::Event event;
    while (window.pollEvent(event)) {
      if (event.type == sf::Event::Closed) {
        window.close();
      }
    }

    f(window);
  }
}

sf::Vector2f engine::resources::mouse_position() const {
  prev_mouse = curr_mouse;
  curr_mouse = sf::Mouse::getPosition(window);
  return sf::Vector2f(curr_mouse);
}

bool engine::resources::mouse_moved() const {
  return curr_mouse != prev_mouse;
}

void engine::resources::set_cursor(sf::Cursor::Type type) const {
  if (cursor_type != type) {
    cursor_type = type;
    if (sf::Cursor cursor; cursor.loadFromSystem(type)) {
      window.setMouseCursor(cursor);
    }
  }
}