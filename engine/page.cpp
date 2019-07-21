//
// Created by marcin on 7/19/19.
//

#include "page.h"

namespace {
// Add an underline or strikethrough line to the vertex array
void addLine(sf::VertexArray &vertices,
             float lineLength,
             float lineTop,
             const sf::Color &color,
             float offset,
             float thickness,
             float outlineThickness = 0) {
  float top{std::floor(lineTop + offset - (thickness / 2) + 0.5f)};
  float bottom{top + std::floor(thickness + 0.5f)};

  vertices.append(sf::Vertex(sf::Vector2f(-outlineThickness, top - outlineThickness), color, sf::Vector2f(1, 1)));
  vertices.append(
      sf::Vertex(sf::Vector2f(lineLength + outlineThickness, top - outlineThickness), color, sf::Vector2f(1, 1)));
  vertices.append(sf::Vertex(sf::Vector2f(-outlineThickness, bottom + outlineThickness), color, sf::Vector2f(1, 1)));
  vertices.append(sf::Vertex(sf::Vector2f(-outlineThickness, bottom + outlineThickness), color, sf::Vector2f(1, 1)));
  vertices.append(
      sf::Vertex(sf::Vector2f(lineLength + outlineThickness, top - outlineThickness), color, sf::Vector2f(1, 1)));
  vertices.append(
      sf::Vertex(sf::Vector2f(lineLength + outlineThickness, bottom + outlineThickness), color, sf::Vector2f(1, 1)));
}

// Add a glyph quad to the vertex array
void addGlyphQuad(sf::VertexArray &vertices,
                  sf::Vector2f position,
                  const sf::Color &color,
                  const sf::Glyph &glyph,
                  float italicShear,
                  float outlineThickness = 0) {
  float padding{1.f};

  float left{glyph.bounds.left - padding};
  float top{glyph.bounds.top - padding};
  float right{glyph.bounds.left + glyph.bounds.width + padding};
  float bottom{glyph.bounds.top + glyph.bounds.height + padding};

  float u1{static_cast<float>(glyph.textureRect.left) - padding};
  float v1{static_cast<float>(glyph.textureRect.top) - padding};
  float u2{static_cast<float>(glyph.textureRect.left + glyph.textureRect.width) + padding};
  float v2{static_cast<float>(glyph.textureRect.top + glyph.textureRect.height) + padding};

  vertices.append(sf::Vertex(
      sf::Vector2f(position.x + left - italicShear * top - outlineThickness, position.y + top - outlineThickness),
      color, sf::Vector2f(u1, v1)));
  vertices.append(sf::Vertex(
      sf::Vector2f(position.x + right - italicShear * top - outlineThickness, position.y + top - outlineThickness),
      color, sf::Vector2f(u2, v1)));
  vertices.append(sf::Vertex(
      sf::Vector2f(position.x + left - italicShear * bottom - outlineThickness, position.y + bottom - outlineThickness),
      color, sf::Vector2f(u1, v2)));
  vertices.append(sf::Vertex(
      sf::Vector2f(position.x + left - italicShear * bottom - outlineThickness, position.y + bottom - outlineThickness),
      color, sf::Vector2f(u1, v2)));
  vertices.append(sf::Vertex(
      sf::Vector2f(position.x + right - italicShear * top - outlineThickness, position.y + top - outlineThickness),
      color, sf::Vector2f(u2, v1)));
  vertices.append(sf::Vertex(sf::Vector2f(position.x + right - italicShear * bottom - outlineThickness,
                                          position.y + bottom - outlineThickness), color, sf::Vector2f(u2, v2)));
}
}

engine::page::page(const page_config_ptr &config, std::vector<paragraph> &&ps) :
    config{config},
    font{config->font},
    font_size{config->font_size},
    current_paragraph{},
    current_character{},
    checked_character{},
    paragraphs{std::move(ps)},
    vertices{sf::Triangles},
    vertices_buffer{sf::Triangles, sf::VertexBuffer::Static},
    bounds{},
    end_of_text{},
    needs_update{true},
    x{config->margin_horizontal},
    y{config->margin_vertical + static_cast<float>(font_size)},
    min_x{config->margin_horizontal + static_cast<float>(font_size)},
    min_y{config->margin_vertical + static_cast<float>(font_size)},
    max_x{},
    max_y{},
    is_bold{},
    is_italic{},
    is_underlined{},
    is_strike_through{},
    is_uppercase{},
    whitespace_width{font->getGlyph(L' ', font_size, false).advance},
    letter_spacing{(whitespace_width / 3.f) * (config->letter_spacing_factor - 1.f)},
    line_spacing{font->getLineSpacing(font_size) * config->line_spacing_factor},
    typing_delay_factor{1.f} {
  whitespace_width += letter_spacing;
  if (paragraphs.empty()) {
    throw std::runtime_error("Paragraphs cannot be empty");
  }
}

void engine::page::add_paragraph(paragraph &&paragraph) {
  paragraphs.emplace_back(std::move(paragraph));
}

void engine::page::reset_paragraphs(std::vector<paragraph> &ps) {
  paragraphs.clear();
  paragraphs.insert(paragraphs.end(),
                    std::make_move_iterator(std::begin(ps)),
                    std::make_move_iterator(std::end(ps)));
}

bool engine::page::text_end() const {
  return end_of_text;
}

void engine::page::advance() {
  const auto &paragraph{paragraphs[current_paragraph]};

  if (current_character == paragraph.length() - 1
      && current_paragraph == paragraphs.size() - 1) {
    end_of_text = true;
  } else {
    if (current_character != paragraph.length() - 1) {
      current_character++;
    } else {
      current_paragraph++;
      checked_character = current_character = 0;
    }
    needs_update = true;
  }
}

sf::FloatRect engine::page::get_local_bounds() const {
  ensure_updated();

  return bounds;
}

void engine::page::draw(sf::RenderTarget &target, sf::RenderStates states) const {
  ensure_updated();

  states.transform *= getTransform();
  states.texture = &font->getTexture(font_size);

  // Update the vertex buffer if it is being used
  if (sf::VertexBuffer::isAvailable()) {
    if (vertices_buffer.getVertexCount() != vertices.getVertexCount())
      vertices_buffer.create(vertices.getVertexCount());

    if (vertices.getVertexCount() > 0)
      vertices_buffer.update(&vertices[0]);
  }

  if (sf::VertexBuffer::isAvailable()) {
    target.draw(vertices_buffer, states);
  } else {
    target.draw(vertices, states);
  }
}

// Measure the length of the next word, if it overflows move it the next line
void engine::page::ensure_line_break(const paragraph &paragraph) const {
  if (current_character <= checked_character)
    return;

  unsigned curr_char{current_character};
  unsigned prev_char{current_character};
  float word_width{};

  do {
    if (paragraph[curr_char] == L'\r')
      continue;

    printf("(%c)", paragraph[curr_char]);

    word_width += font->getKerning(paragraph[prev_char], paragraph[curr_char], font_size);

    switch (paragraph[curr_char]) {
      case L' ':
        word_width += whitespace_width;
        break;
      case L'\t':
        word_width += whitespace_width * 4;
        break;
      default:
        const auto &glyph{font->getGlyph(paragraph[curr_char], font_size, false)};
        word_width += glyph.advance + letter_spacing;
        break;
    }

    checked_character = prev_char = curr_char;
  } while (!std::iswspace(paragraph[curr_char++]));

  printf("\nx: %f, y: %f, word_width: %f\n", x, y, word_width);

  if (x + word_width + config->margin_horizontal >= config->page_width) {
    new_line();

    min_x = std::min(min_x, x);
    min_y = std::min(min_y, y);
    max_x = std::max(max_x, x);
    max_y = std::max(max_y, y);

    update_bounds();
  }
}

void engine::page::ensure_updated() const {
  if (!needs_update)
    return;

  needs_update = false;

  auto &paragraph{paragraphs[current_paragraph]};
  wchar_t prev_char{paragraph[current_character ? current_character - 1u : 0u]};
  wchar_t curr_char{paragraph[current_character]};

  // If this is the new paragraph, add a newline
  if (current_paragraph && !current_character)
    new_line();

  // Skip to avoid glitches
  if (curr_char == L'\r')
    return;

  // Ensure line is broken if next word exceeds page width
  ensure_line_break(paragraph);
  // Load text effects starting at this position
  apply_text_effects(paragraph);

  x += font->getKerning(prev_char, curr_char, font_size);

  // Handle special characters
  if ((curr_char == L' ') || (curr_char == L'\n') || (curr_char == L'\t')) {
    min_x = std::min(min_x, x);
    min_y = std::min(min_y, y);

    switch (curr_char) {
      case L' ':
        x += whitespace_width;
        break;
      case L'\t':
        x += whitespace_width * 4;
        break;
      case L'\n':
        y += line_spacing;
        x = config->margin_horizontal;
        break;
    }

    max_x = std::max(max_x, x);
    max_y = std::max(max_y, y);

    delay();
    return;
  }

  const auto &glyph = font->getGlyph(curr_char, font_size, false);
  addGlyphQuad(vertices, sf::Vector2f(x, y), sf::Color::White, glyph, 0.f);
  x += glyph.advance + letter_spacing;

  update_bounds();

  // Remove text events ending at this position
  remove_text_effects(paragraph);

  delay();
}

void engine::page::apply_text_effects(paragraph &paragraph) const {
  paragraph.add_starting_effects(current_character);
  // Set instance variables to ensure text effects are rendered
}

void engine::page::remove_text_effects(paragraph &paragraph) const {
  // Remove text effects which end at the current character
  // Reset all instance variables defining text style
  paragraph.remove_ending_effects(current_character);
}

void engine::page::update_bounds() const {
  bounds.left = min_x;
  bounds.top = min_y;
  bounds.width = max_x - min_x;
  bounds.height = max_y - min_y;
}

void engine::page::delay() const {
  sf::Clock clock;
  sf::Time time;

  while (time.asMilliseconds() < config->typing_delay * typing_delay_factor) {
    time += clock.getElapsedTime();
  }
}