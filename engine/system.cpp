//
// Created by marcin on 7/21/19.
//

#include "system.hpp"

engine::system::system(
    sf::VideoMode mode,
    const std::string &fonts_path,
    const std::string &sounds_path,
    const std::string &textures_path,
    const std::string &configs_path
)
    : mode{mode},
      current_cursor{cursor::ARROW},
      arrow_cursor{},
      grab_cursor{},
      curr_mouse{},
      prev_mouse{},
      window{mode, "Typewriter Gaem"},
      font{},
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
      configs{
          load_resources<pt::ptree>(
              configs_path,
              [](const auto &path) -> std::optional<pt::ptree> {
                pt::ptree pt;
                try {
                  pt::json_parser::read_json(path, pt);
                } catch (const pt::json_parser_error &e) {
                  throw std::runtime_error("Could not parse JSON config file: " + path);
                }
                return pt;
              }
          )
      },
      mouse_pressed{},
      mouse_pressed_position{} {
  const auto &page_config{get_configs("page")};
  font_size = page_config.get<unsigned>("page.font_size");
  page_width = page_config.get<float>("page.page_width");
  page_height = page_config.get<float>("page.page_height");
  margin_vertical = page_config.get<float>("page.margin_vertical");
  margin_horizontal = page_config.get<float>("page.margin_horizontal");
  letter_spacing_factor = page_config.get<float>("page.letter_spacing_factor");
  line_spacing_factor = page_config.get<float>("page.line_spacing_factor");
  typing_delay = page_config.get<unsigned>("page.typing_delay") * 100000u;

  auto page_font{page_config.get<std::string>("page.font")};
  font = &get_fonts(page_font);

  if (!arrow_cursor.loadFromSystem(sf::Cursor::Type::Arrow)) {
    throw std::runtime_error("Arrow cursor loading error");
  }

  if (!grab_cursor.loadFromSystem(sf::Cursor::Type::Hand)) {
    throw std::runtime_error("Grab cursor loading error");
  }

  whitespace_width = font->getGlyph(L' ', font_size, false).advance;
  letter_spacing = (whitespace_width / 3.f) * (letter_spacing_factor - 1.f);
  line_spacing = font->getLineSpacing(font_size) * line_spacing_factor;
  line_spacing_margin = line_spacing / 1.3f;
  underline_offset = font->getUnderlinePosition(font_size);
  underline_thickness = font->getUnderlineThickness(font_size);

  whitespace_width += letter_spacing;
}

void engine::system::delay(float duration, float delay_factor) const {
  sf::Clock clock;
  sf::Time time;

  while (time.asMilliseconds() < duration * delay_factor) {
    time += clock.getElapsedTime();
  }
}

bool engine::system::mouse_moved() const {
  return curr_mouse != prev_mouse;
}

const sf::Vector2f &engine::system::mouse_position() const {
  prev_mouse = curr_mouse;
  curr_mouse = sf::Vector2f{sf::Mouse::getPosition(window)};
  return curr_mouse;
}

void engine::system::set_mouse_click(const sf::Vector2f &pos) const {
  mouse_pressed = true;
  mouse_pressed_position = pos;
}

bool engine::system::mouse_click_available() const {
  return mouse_pressed;
}

const sf::Vector2f &engine::system::mouse_click_position() const {
  mouse_pressed = false;
  return mouse_pressed_position;
}

float engine::system::effective_page_width() const {
  return page_width - 2 * margin_horizontal;
}

float engine::system::effective_page_height() const {
  return page_height - 2 * margin_vertical;
}

void engine::system::set_cursor(cursor c) const {
  if (c == current_cursor) return;
  current_cursor = c;
  switch (c) {
    case cursor::ARROW:
      window.setMouseCursor(arrow_cursor);
      break;
    case cursor::HAND:
      window.setMouseCursor(grab_cursor);
      break;
  }
}

sf::RenderWindow &engine::system::get_window() const {
  return window;
}