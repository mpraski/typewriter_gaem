//
// Created by marcin on 10/21/19.
//

#include "General.hpp"

sf::Uint64 engine::gen::next_uid() {
  static sf::Uint64 uid{1u};
  return uid++;
}

void engine::gen::addGlyphQuad(sf::VertexArray &vertices,
                               sf::Vector2f position,
                               const sf::Color &color,
                               const sf::Glyph &glyph,
                               float italicShear) {
  float padding{1.f};

  float left{glyph.bounds.left - padding};
  float top{glyph.bounds.top - padding};
  float right{glyph.bounds.left + glyph.bounds.width + padding};
  float bottom{glyph.bounds.top + glyph.bounds.height + padding};

  float u1{static_cast<float>(glyph.textureRect.left) - padding};
  float v1{static_cast<float>(glyph.textureRect.top) - padding};
  float u2{static_cast<float>(glyph.textureRect.left + glyph.textureRect.width) + padding};
  float v2{static_cast<float>(glyph.textureRect.top + glyph.textureRect.height) + padding};

  vertices.append(
      sf::Vertex(sf::Vector2f(position.x + left - italicShear * top, position.y + top), color, sf::Vector2f(u1, v1)));
  vertices.append(
      sf::Vertex(sf::Vector2f(position.x + right - italicShear * top, position.y + top), color, sf::Vector2f(u2, v1)));
  vertices.append(sf::Vertex(sf::Vector2f(position.x + left - italicShear * bottom, position.y + bottom), color,
                             sf::Vector2f(u1, v2)));
  vertices.append(sf::Vertex(sf::Vector2f(position.x + left - italicShear * bottom, position.y + bottom), color,
                             sf::Vector2f(u1, v2)));
  vertices.append(
      sf::Vertex(sf::Vector2f(position.x + right - italicShear * top, position.y + top), color, sf::Vector2f(u2, v1)));
  vertices.append(sf::Vertex(sf::Vector2f(position.x + right - italicShear * bottom, position.y + bottom), color,
                             sf::Vector2f(u2, v2)));
}

void engine::gen::addRect(sf::VertexArray &vertices, const sf::FloatRect &rect, sf::Color color) {
  vertices.append(
      sf::Vertex(sf::Vector2f(rect.left, rect.top), color,
                 sf::Vector2f(1, 1)));
  vertices.append(sf::Vertex(
      sf::Vector2f(rect.left, rect.top + rect.height),
      color, sf::Vector2f(1, 1)));
  vertices.append(sf::Vertex(
      sf::Vector2f(rect.left, rect.top + rect.height),
      color, sf::Vector2f(1, 1)));
  vertices.append(sf::Vertex(
      sf::Vector2f(rect.left + rect.width,
                   rect.top + rect.height),
      color, sf::Vector2f(1, 1)));
  vertices.append(sf::Vertex(
      sf::Vector2f(rect.left + rect.width,
                   rect.top + rect.height),
      color, sf::Vector2f(1, 1)));
  vertices.append(sf::Vertex(
      sf::Vector2f(rect.left + rect.width, rect.top),
      color, sf::Vector2f(1, 1)));
  vertices.append(sf::Vertex(
      sf::Vector2f(rect.left + rect.width, rect.top),
      color, sf::Vector2f(1, 1)));
  vertices.append(
      sf::Vertex(sf::Vector2f(rect.left, rect.top), color,
                 sf::Vector2f(1, 1)));
}

void engine::gen::addFontBackground(sf::VertexArray &vertices,
                                    sf::Vector2f position,
                                    const sf::Glyph &glyph,
                                    float italicShear) {
  float padding{1.f};

  float left{glyph.bounds.left - padding};
  float top{glyph.bounds.top - padding};
  float right{glyph.bounds.left + glyph.bounds.width + padding};
  float bottom{glyph.bounds.top + glyph.bounds.height + padding};

  vertices.append(
      sf::Vertex(sf::Vector2f(position.x + left - italicShear * top, position.y + top), sf::Vector2f(0, 0)));
  vertices.append(
      sf::Vertex(sf::Vector2f(position.x + right - italicShear * top, position.y + top),
                 sf::Vector2f(glyph.bounds.width + padding, 0)));
  vertices.append(sf::Vertex(sf::Vector2f(position.x + left - italicShear * bottom, position.y + bottom),
                             sf::Vector2f(0, glyph.bounds.height + padding)));
  vertices.append(sf::Vertex(sf::Vector2f(position.x + left - italicShear * bottom, position.y + bottom),
                             sf::Vector2f(0, glyph.bounds.height + padding)));
  vertices.append(
      sf::Vertex(sf::Vector2f(position.x + right - italicShear * top, position.y + top),
                 sf::Vector2f(glyph.bounds.width + padding, 0)));
  vertices.append(sf::Vertex(sf::Vector2f(position.x + right - italicShear * bottom, position.y + bottom),
                             sf::Vector2f(glyph.bounds.width + padding, glyph.bounds.height + padding)));
}

void engine::gen::addLine(sf::VertexArray &vertices,
                          float x,
                          float width,
                          float lineTop,
                          const sf::Color &color,
                          float offset,
                          float thickness) {
  float top{std::floor(lineTop + offset - (thickness / 2) + 0.5f)};
  float bottom{top + std::floor(thickness + 0.5f)};

  vertices.append(sf::Vertex(sf::Vector2f(x, top), color, sf::Vector2f(1, 1)));
  vertices.append(sf::Vertex(sf::Vector2f(x + width, top), color, sf::Vector2f(1, 1)));
  vertices.append(sf::Vertex(sf::Vector2f(x, bottom), color, sf::Vector2f(1, 1)));
  vertices.append(sf::Vertex(sf::Vector2f(x, bottom), color, sf::Vector2f(1, 1)));
  vertices.append(sf::Vertex(sf::Vector2f(x + width, top), color, sf::Vector2f(1, 1)));
  vertices.append(sf::Vertex(sf::Vector2f(x + width, bottom), color, sf::Vector2f(1, 1)));
}
