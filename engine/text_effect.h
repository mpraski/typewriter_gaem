//
// Created by marcin on 7/20/19.
//

#ifndef TYPEWRITER_GAEM_TEXT_EFFECT_H
#define TYPEWRITER_GAEM_TEXT_EFFECT_H

#include <type_traits>
#include <functional>

namespace engine {
class text_effect {
public:
    enum class kind {
        BOLD,
        ITALIC,
        UPPERCASE,
        UNDERLINE,
        STRIKE_THROUGH,
        DELAY,
        PACE_CHANGE,
        CENTER,
    };

    kind kind;
    unsigned begin;
    unsigned end;
    float delay;
    float pace;
};
}

namespace std {
template<>
class hash<engine::text_effect> {
public :
    size_t operator()(const engine::text_effect &name) const {
      return static_cast<size_t>(to_integral(name.kind));
    }

private:
    template<typename E>
    static constexpr auto to_integral(E e) -> typename std::underlying_type<E>::type {
      return static_cast<typename std::underlying_type<E>::type>(e);
    }
};
};

#endif //TYPEWRITER_GAEM_TEXT_EFFECT_H
