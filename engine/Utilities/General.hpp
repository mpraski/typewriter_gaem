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
#include <typeindex>
#include <boost/log/sources/record_ostream.hpp>

namespace engine::gen {
template<typename E, typename T, typename ... Ts>
inline void to_stream(std::basic_ostream<E> &os, T &&t, Ts &&... ts) {
  os << std::forward<T>(t);
  (void) std::initializer_list<int>{0, (void(os << std::forward<Ts>(ts)), 0)...};
}

template<typename E, typename T, typename ... Ts>
inline void to_stream(boost::log::basic_record_ostream<E> &os, T &&t, Ts &&... ts) {
  os << std::forward<T>(t);
  (void) std::initializer_list<int>{0, (void(os << std::forward<Ts>(ts)), 0)...};
}

template<typename T, typename ... Ts>
auto str(T &&t, Ts &&... ts) {
  std::ostringstream os;
  to_stream(os, std::forward<T>(t), std::forward<Ts>(ts)...);
  return os.str();
}

template<typename T>
const decltype(T{}) &default_object() {
  static T x{};
  return x;
}

template<
    typename T,
    typename K = typename T::key_type,
    typename E = typename T::mapped_type
>
constexpr auto find_default(T &map, const K &key, const E &e = gen::default_object<E>()) {
  if (auto it{map.find(key)}; it != std::end(map)) {
    return it;
  }
  auto[it, ok]{map.insert(std::make_pair(key, e))};
  assert(ok);
  return it;
}

template<class T, class F>
constexpr void remove_if(T &ts, F &&f) {
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
constexpr auto find_if(const T &ts, F &&f) {
  return std::find_if(
      std::cbegin(ts),
      std::cend(ts),
      std::forward<F>(f)
  );
}

template<class T, class F>
constexpr auto find_if(T &ts, F &&f) {
  return std::find_if(
      std::begin(ts),
      std::end(ts),
      std::forward<F>(f)
  );
}

template<class T, class F>
constexpr auto find(const T &ts, F &&f) {
  return std::find(
      std::cbegin(ts),
      std::cend(ts),
      std::forward<F>(f)
  );
}

template<class T, class F>
constexpr auto find(T &ts, F &&f) {
  return std::find(
      std::begin(ts),
      std::end(ts),
      std::forward<F>(f)
  );
}

template<class T, class F>
constexpr auto for_each(T &ts, F &&f) {
  return std::for_each(
      std::begin(ts),
      std::end(ts),
      std::forward<F>(f)
  );
}

template<typename E, typename It>
void split(const std::basic_string<E> &s, E delim, It result) {
  std::basic_stringstream<E> ss(s);
  std::basic_string<E> item;
  while (std::getline(ss, item, delim)) {
    *(result++) = item;
  }
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
constexpr auto type_id() noexcept {
  return std::type_index(typeid(T));
}

template<class T>
constexpr auto to_uintptr(const T *t) noexcept {
  return reinterpret_cast<std::uintptr_t>(t);
}

template<class It, class Fun>
It random_iter(It begin, It end, Fun &&randFun) {
  long sz{std::distance(begin, end)};
  long div{RAND_MAX / sz};

  long k;
  do { k = randFun() / div; } while (k >= sz);

  return std::next(begin, k);
}

template<
    typename E,
    typename std::underlying_type<E>::type First,
    typename std::underlying_type<E>::type Last,
    typename Pred,
    typename Fun
>
constexpr void if_else(Pred &&p, Fun &&f) {
  if constexpr (First < Last) {
    if (p(static_cast<E>(First))) {
      f(static_cast<E>(First));
    } else {
      if_else<E, First + 1, Last>(std::forward<Pred>(p), std::forward<Fun>(f));
    }
  }
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
