//
// Created by marcin on 7/30/19.
//

#ifndef TYPEWRITER_GAEM_GENERAL_HPP
#define TYPEWRITER_GAEM_GENERAL_HPP

#include <sstream>
#include <algorithm>
#include <variant>
#include <SFML/Graphics.hpp>
#include <cmath>

namespace engine::gen {
template<typename T, typename ... Ts>
auto str(T &&t, Ts &&... ts) {
  std::ostringstream os;

  os << std::forward<T>(t);
  (void) std::initializer_list<int>{0, (void(os << std::forward<Ts>(ts)), 0)...};

  return os.str();
}

template<class Base, class T>
bool instanceof(const T *ptr) {
  return dynamic_cast<const Base *>(ptr) != nullptr;
}

template<class T, class F>
void remove_if(T &ts, F &&f) {
  ts.erase(
      std::remove_if(
          std::begin(ts),
          std::end(ts),
          std::forward<F>(f)
      ),
      std::end(ts)
  );
}

template<class T, class F>
auto find_if(const T &ts, F &&f) {
  return std::find_if(
      std::cbegin(ts),
      std::cend(ts),
      std::forward<F>(f)
  );
}

template<class T, class F>
auto find_if(T &ts, F &&f) {
  return std::find_if(
      std::begin(ts),
      std::end(ts),
      std::forward<F>(f)
  );
}

template<class T, class F>
auto find(const T &ts, F &&f) {
  return std::find(
      std::cbegin(ts),
      std::cend(ts),
      std::forward<F>(f)
  );
}

template<class T, class F>
auto find(T &ts, F &&f) {
  return std::find(
      std::begin(ts),
      std::end(ts),
      std::forward<F>(f)
  );
}

template<class T, class F>
auto for_each(T &ts, F &&f) {
  return std::for_each(
      std::begin(ts),
      std::end(ts),
      std::forward<F>(f)
  );
}

template<typename It>
void split(const std::string &s, char delim, It result) {
  std::stringstream ss(s);
  std::string item;
  while (std::getline(ss, item, delim)) {
    *(result++) = item;
  }
}

template<typename It>
void wsplit(const std::wstring &s, wchar_t delim, It result) {
  std::wstringstream wss(s);
  std::wstring item;
  while (std::getline(wss, item, delim)) {
    *(result++) = item;
  }
}

template<class T>
constexpr auto last(const T &t) {
  return std::prev(std::cend(t));
}

template<class T>
constexpr auto last(T &t) {
  return std::prev(std::end(t));
}

template<class E>
constexpr auto to_underlying(E e) noexcept {
  return static_cast<typename std::underlying_type<E>::type>(e);
}

template<class T>
constexpr auto is_zero(T t) {
  static_assert(std::is_floating_point_v<T>);
  const constexpr auto eps{1.0e-10};
  return fabs(t) < eps;
}

template<class... Ts>
constexpr void draw(sf::RenderTarget &target, const sf::RenderStates &states, Ts &&... ts) {
  (target.draw(std::forward<Ts>(ts), states), ...);
}

template<class T>
constexpr auto type_id() {
  return typeid(T).hash_code();
}

template<class T>
constexpr auto to_uintptr(const T *t) {
  return reinterpret_cast<std::uintptr_t>(t);
}

auto next_uid() {
  static sf::Uint64 uid{1u};
  return uid++;
}

// Add an underline or strikethrough line to the vertex array
void addLine(sf::VertexArray &vertices,
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

void addFontBackground(sf::VertexArray &vertices,
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

// Add a glyph quad to the vertex array
void addGlyphQuad(sf::VertexArray &vertices,
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

void addRect(sf::VertexArray &vertices, const sf::FloatRect &rect, sf::Color color = sf::Color::Green) {
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
}

#endif //TYPEWRITER_GAEM_GENERAL_HPP
