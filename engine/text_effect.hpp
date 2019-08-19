//
// Created by marcin on 7/20/19.
//

#ifndef TYPEWRITER_GAEM_TEXT_EFFECT_HPP
#define TYPEWRITER_GAEM_TEXT_EFFECT_HPP

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
        SPACING,
        COLOR,
        TEXTURE,
        CENTER,
        RIGHT,
        FONT_SIZE
    };

    text_effect(
        kind kind,
        size_t begin,
        size_t end,
        float delay_factor = 1.f,
        float letter_spacing_factor = 0.f,
        sf::Color color = sf::Color::White,
        std::string texture = "",
        float font_size_factor = 1.f
    );

    text_effect &with_delay(float df);

    text_effect &with_letter_spacing(float ls);

    text_effect &with_color(sf::Color c);

    text_effect &with_texture(std::string t);

    text_effect &with_font_size(float fz);

    inline bool operator==(const text_effect &other) const noexcept {
      return kind == other.kind
             && begin == other.begin
             && end == other.end;
    }

    kind kind;
    size_t begin;
    size_t end;
    float delay_factor;
    float letter_spacing_factor;
    sf::Color color;
    std::string texture;
    float font_size_factor;
};
}

namespace std {
template<>
class hash<engine::text_effect> {
public :
    size_t operator()(const engine::text_effect &e) const {
      return static_cast<size_t>(to_integral(e.kind)) ^ e.begin ^ e.end;
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
      return l == r;
    }
};
}

#endif //TYPEWRITER_GAEM_TEXT_EFFECT_HPP
