//
// Created by marcin on 7/19/19.
//

#include "page.h"
#include <cmath>
#include "SFML/Graphics.hpp"

namespace {
// Add an underline or strikethrough line to the vertex array
void addLine(sf::VertexArray &vertices,
             float lineLength,
             float lineTop,
             const sf::Color &color,
             float offset,
             float thickness,
             float outlineThickness = 0) {
  float top = std::floor(lineTop + offset - (thickness / 2) + 0.5f);
  float bottom = top + std::floor(thickness + 0.5f);

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
  float padding = 1.0;

  float left = glyph.bounds.left - padding;
  float top = glyph.bounds.top - padding;
  float right = glyph.bounds.left + glyph.bounds.width + padding;
  float bottom = glyph.bounds.top + glyph.bounds.height + padding;

  float u1 = static_cast<float>(glyph.textureRect.left) - padding;
  float v1 = static_cast<float>(glyph.textureRect.top) - padding;
  float u2 = static_cast<float>(glyph.textureRect.left + glyph.textureRect.width) + padding;
  float v2 = static_cast<float>(glyph.textureRect.top + glyph.textureRect.height) + padding;

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

engine::page::page(const std::shared_ptr<sf::Font> &font,
                   size_t font_size,
                   size_t page_width) :
    font{font},
    font_size{font_size},
    page_width{page_width},
    current_paragraph{0},
    current_character{},
    paragraphs{},
    vertices{sf::Triangles},
    vertices_buffer{sf::Triangles, sf::VertexBuffer::Static},
    should_flip_new{false},
    letter_spacing_factor{1.f},
    line_spacing_factor{1.f},
    whitespace_width{font->getGlyph(L' ', font_size, false).advance},
    letter_spacing{(whitespace_width / 3.f) * (letter_spacing_factor - 1.f)},
    line_spacing{font->getLineSpacing(font_size) * line_spacing_factor},
    x{0.f},
    y{static_cast<float>(font_size)},
    min_x{static_cast<float>(font_size)},
    max_x{0.f},
    min_y{static_cast<float>(font_size)},
    max_y{0.f} {
  whitespace_width += letter_spacing;
}

void engine::page::increment_character() {
  if (++current_character == paragraphs[current_paragraph].size()) {
    if (++current_paragraph == paragraphs.size()) {
      should_flip_new = true;
    }
  }
}

// Implement text wrapping
void engine::page::draw(sf::RenderTarget &target, sf::RenderStates states) {
  auto &paragraph{paragraphs[current_paragraph]};

  wchar_t prev_char{paragraph[current_character - 1]};
  wchar_t curr_char{paragraph[current_character]};

  if (curr_char == L'\r')
    return;


  x += font->getKerning(prev_char, curr_char, font_size);

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
        x = 0;
        break;
    }

    max_x = std::max(max_x, x);
    max_y = std::max(max_y, y);

    return;
  }

  const auto &glyph = font->getGlyph(curr_char, font_size, false);

  addGlyphQuad(vertices, sf::Vector2f(x, y), sf::Color::White, glyph, 0.f);

  x += glyph.advance + letter_spacing;

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

/*
void engine::page::wrap_words(std::wstring &paragraph) const {
  size_t line_width{0};
  size_t word_width{0};
  size_t last_space{0};

  // To-Do: Should kerning be taken into consideration?
  for (size_t i{0}; i < paragraph.length(); ++i) {
    auto glyph_width{font->getGlyph(paragraph[i], font_size, false).textureRect.width};
    line_width += glyph_width;
    word_width += glyph_width;

    if (std::iswspace(paragraph[i])) {
      if (line_width >= page_width) {
        paragraph[last_space] = L'\n';
        line_width = word_width;
      }

      last_space = i;
      word_width = 0;
    }
  }
}*/