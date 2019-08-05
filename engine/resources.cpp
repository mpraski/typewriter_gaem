//
// Created by marcin on 7/21/19.
//

#include "resources.h"

engine::resources::resources(
    sf::VideoMode mode,
    const std::string &fonts_path,
    const std::string &sounds_path,
    const std::string &textures_path,
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
      },
      textures{
          load_resources<sf::Texture>(
              textures_path,
              [](const auto &path) -> std::optional<sf::Texture> {
                if (sf::Texture texture; texture.loadFromFile(path)) {
                  return texture;
                }
                return {};
              }
          )
      },
      mouse_pressed{},
      mouse_pressed_position{} {
  font = &fonts[ROOT_RESOURCE_CATEGORY][DEFAULT_FONT];

  whitespace_width = font->getGlyph(L' ', font_size, false).advance;
  letter_spacing = (whitespace_width / 3.f) * (letter_spacing_factor - 1.f);
  line_spacing = font->getLineSpacing(font_size) * line_spacing_factor;
  line_spacing_margin = line_spacing / 1.3f;
  underline_offset = font->getUnderlinePosition(font_size);
  underline_thickness = font->getUnderlineThickness(font_size);

  whitespace_width += letter_spacing;
}

void engine::resources::display(const std::function<void(sf::RenderWindow &)> &f) const {
  bool holds_mouse{false};

  while (window.isOpen()) {
    sf::Event event;
    while (window.pollEvent(event)) {
      switch (event.type) {
        case sf::Event::Closed:
          window.close();
          break;
        case sf::Event::MouseButtonPressed:
          holds_mouse = true;
          break;
        case sf::Event::MouseButtonReleased:
          if (holds_mouse) {
            mouse_pressed = true;
            mouse_pressed_position = sf::Vector2f{sf::Mouse::getPosition(window)};
            holds_mouse = false;
          }
          break;
        default:
          break;
      }
    }

    f(window);
  }
}

bool engine::resources::mouse_moved() const {
  return curr_mouse != prev_mouse;
}

const sf::Vector2f &engine::resources::mouse_position() const {
  prev_mouse = curr_mouse;
  curr_mouse = sf::Vector2f{sf::Mouse::getPosition(window)};
  return curr_mouse;
}

bool engine::resources::mouse_click_available() const {
  return mouse_pressed;
}

const sf::Vector2f &engine::resources::mouse_click_position() const {
  mouse_pressed = false;
  return mouse_pressed_position;
}

bool engine::resources::visible(sf::FloatRect bounds) const {
  return false;
}

float engine::resources::effective_page_width() const {
  return page_width - 2 * margin_horizontal;
}