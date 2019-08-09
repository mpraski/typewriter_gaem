//
// Created by marcin on 8/8/19.
//

#ifndef TYPEWRITER_GAEM_TUPLES_HPP
#define TYPEWRITER_GAEM_TUPLES_HPP

#include <cstddef>
#include <tuple>

namespace engine {
template<class T, size_t N, size_t... I>
constexpr decltype(auto) _as_tuple(const std::array<T, N> &arr, std::index_sequence<I...>) {
  return std::make_tuple(T{arr[I]}...);
}

template<class... Ts>
constexpr decltype(auto) as_tuple(const std::tuple<Ts...> &tuple) {
  return tuple;
}

template<class T>
constexpr decltype(auto) as_tuple(T &&t) {
  return std::make_tuple<T>(std::forward<T>(t));
}

template<class T, size_t N, size_t... I>
constexpr decltype(auto) as_tuple(const std::array<T, N> &arr) {
  return _as_tuple(arr, std::make_index_sequence<N>{});
}

template<size_t I, class... T>
constexpr decltype(auto) addi(const std::tuple<T...> &a, const std::tuple<T...> &b) {
  return std::get<I>(a) + std::get<I>(b);
}

template<std::size_t... I, class... T>
constexpr decltype(auto) add(std::index_sequence<I...>, const std::tuple<T...> &a, const std::tuple<T...> &b) {
  return std::make_tuple(addi<I>(a, b)...);
}

template<typename... T>
constexpr decltype(auto) operator+(const std::tuple<T...> &a, const std::tuple<T...> &b) {
  return add(std::index_sequence_for<T...>{}, a, b);
}

template<size_t I, class... T>
constexpr decltype(auto) minusi(const std::tuple<T...> &a, const std::tuple<T...> &b) {
  return std::get<I>(a) - std::get<I>(b);
}

template<std::size_t... I, class... T>
constexpr decltype(auto) minus(std::index_sequence<I...>, const std::tuple<T...> &a, const std::tuple<T...> &b) {
  return std::make_tuple(minusi<I>(a, b)...);
}

template<typename... T>
constexpr decltype(auto) operator-(const std::tuple<T...> &a, const std::tuple<T...> &b) {
  return minus(std::index_sequence_for<T...>{}, a, b);
}
}

#endif //TYPEWRITER_GAEM_TUPLES_HPP
