//
// Created by marcin on 7/30/19.
//

#ifndef TYPEWRITER_GAEM_GENERAL_H
#define TYPEWRITER_GAEM_GENERAL_H

#include <sstream>
#include <algorithm>

namespace engine {
class general {
public:
    template<typename T, typename ... Ts>
    static auto str(T &&t, Ts &&... ts) {
      std::ostringstream os;

      os << std::forward<T>(t);
      (void) std::initializer_list<int>{0, (void(os << std::forward<Ts>(ts)), 0)...};

      return os.str();
    }

    template<class Base, class T>
    static bool instanceof(const T *ptr) {
      return dynamic_cast<const Base *>(ptr) != nullptr;
    }

    template<class T, class F>
    static constexpr void remove_if(T &ts, F &&f) {
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
    static constexpr auto find(const T &ts, F &&f) {
      return std::find_if(
          std::cbegin(ts),
          std::cend(ts),
          std::forward<F>(f)
      );
    }

    template<class T, class F>
    static constexpr auto find(T &ts, F &&f) {
      return std::find_if(
          std::begin(ts),
          std::end(ts),
          std::forward<F>(f)
      );
    }

    template<class T, class F>
    static constexpr auto for_each(T &ts, F &&f) {
      return std::for_each(
          std::begin(ts),
          std::end(ts),
          std::forward<F>(f)
      );
    }

    template<typename It>
    static void split(const std::string &s, char delim, It result) {
      std::stringstream ss(s);
      std::string item;
      while (std::getline(ss, item, delim)) {
        *(result++) = item;
      }
    }

    template<typename It>
    static void wsplit(const std::wstring &s, wchar_t delim, It result) {
      std::wstringstream wss(s);
      std::wstring item;
      while (std::getline(wss, item, delim)) {
        *(result++) = item;
      }
    }

    template<class T>
    static inline auto last(const T &t) {
      return std::prev(std::cend(t));
    }

    template<class T>
    static inline auto last(T &t) {
      return std::prev(std::end(t));
    }

    template<class E>
    static constexpr auto to_underlying(E e) noexcept {
      return static_cast<typename std::underlying_type<E>::type>(e);
    }

};
}

#endif //TYPEWRITER_GAEM_GENERAL_H
