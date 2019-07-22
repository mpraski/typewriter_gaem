//
// Created by marcin on 7/20/19.
//

#ifndef TYPEWRITER_GAEM_TEXT_EFFECT_H
#define TYPEWRITER_GAEM_TEXT_EFFECT_H

#include <type_traits>
#include <functional>
#include <SFML/Graphics.hpp>

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
        CENTER,
        SPACING,
        COLOR
    };

    text_effect(
        kind kind,
        unsigned begin,
        unsigned end,
        float delay_factor = 1.f,
        float letter_spacing_factor = 0.f,
        sf::Color color = sf::Color::White
    );

    text_effect &with_delay(float df = 1.f);

    text_effect &with_letter_spacing(float ls = 1.f);

    text_effect &with_color(sf::Color c = sf::Color::White);

    kind kind;
    unsigned begin;
    unsigned end;
    float delay_factor;
    float letter_spacing_factor;
    sf::Color color;
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

template<>
class equal_to<engine::text_effect> {
public:
    bool operator()(const engine::text_effect &l, const engine::text_effect &r) const {
      return l.kind == r.kind;
    }
};
};

#endif //TYPEWRITER_GAEM_TEXT_EFFECT_H
