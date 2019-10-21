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

template<typename T>
decltype(T{}) &default_object() {
  static T x{};
  return x;
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

sf::Uint64 next_uid();

// Add an underline or strikethrough line to the vertex array
void addLine(sf::VertexArray &vertices,
             float x,
             float width,
             float lineTop,
             const sf::Color &color,
             float offset,
             float thickness);

void addFontBackground(sf::VertexArray &vertices,
                       sf::Vector2f position,
                       const sf::Glyph &glyph,
                       float italicShear);

// Add a glyph quad to the vertex array
void addGlyphQuad(sf::VertexArray &vertices,
                  sf::Vector2f position,
                  const sf::Color &color,
                  const sf::Glyph &glyph,
                  float italicShear);

void addRect(sf::VertexArray &vertices, const sf::FloatRect &rect, sf::Color color = sf::Color::Green);
}

#endif //TYPEWRITER_GAEM_GENERAL_HPP
