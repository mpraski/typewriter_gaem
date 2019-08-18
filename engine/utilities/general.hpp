//
// Created by marcin on 7/30/19.
//

#ifndef TYPEWRITER_GAEM_GENERAL_HPP
#define TYPEWRITER_GAEM_GENERAL_HPP

#include <sstream>
#include <algorithm>
#include <variant>
#include <SFML/Graphics/RenderTarget.hpp>

namespace engine::gen {
template<typename T, typename ... Ts>
auto str(T &&t, Ts &&... ts) {
  std::ostringstream os;

  os << std::forward<T>(t);
  (void) std::initializer_list<int>{0, (void(os << std::forward<Ts>(ts)), 0)...};

  return os.str();
}

template<class Base, class T>
constexpr bool instanceof(const T *ptr) {
  return dynamic_cast<const Base *>(ptr) != nullptr;
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
constexpr auto find(const T &ts, F &&f) {
  return std::find_if(
      std::cbegin(ts),
      std::cend(ts),
      std::forward<F>(f)
  );
}

template<class T, class F>
constexpr auto find(T &ts, F &&f) {
  return std::find_if(
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
}

#endif //TYPEWRITER_GAEM_GENERAL_HPP
