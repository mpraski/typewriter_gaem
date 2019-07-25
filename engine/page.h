//
// Created by marcin on 7/19/19.
//

#ifndef SFML_GAME_PAGE_H
#define SFML_GAME_PAGE_H

#include <vector>
#include <string>
#include <cwctype>
#include <memory>
#include <cmath>
#include <unordered_set>
#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "game_state.h"
#include "sounds.h"
#include "printables/printable.h"
#include "utilities/text_buffer.h"

#define pointer(IT) std::get<0>(*(IT))
#define rect(IT) std::get<1>(*(IT))
#define index(IT) std::get<2>(*(IT))

namespace engine {
class page : public game_state, public sf::Drawable, public sf::Transformable {
    using printable_array = std::vector<std::tuple<std::unique_ptr<printable>, sf::FloatRect, size_t>>;
    using printable_iterator = printable_array::iterator;
    using effect_array = std::vector<text_effect>;
    using effect_map = std::unordered_map<size_t, std::vector<text_effect>>;
public:
    page(const resources_ptr &rptr, std::vector<printable *> &&ps);

    bool text_end() const;

    void advance();

    void input();

private:
    sounds audio;
    mutable text_buffer buffer;
    // Core data
    mutable printable_array printables;
    mutable effect_array active_effects;
    mutable effect_array new_effects;
    mutable effect_map global_effects;
    // Character and paragraph cursor
    printable_iterator current_printable;
    size_t current_character;
    mutable size_t checked_character;
    // SFML buffers
    mutable sf::VertexArray vertices;
    mutable sf::VertexBuffer vertices_buffer;
    mutable sf::FloatRect bounds;
    // Control flags
    mutable bool end_of_text;
    mutable bool needs_update;
    // Text bounds
    mutable float x;
    mutable float y;
    mutable float min_x;
    mutable float min_y;
    mutable float max_x;
    mutable float max_y;
    // Text properties
    mutable bool is_bold;
    mutable bool is_underlined;
    mutable bool is_strike_through;
    mutable bool is_uppercase;
    mutable bool is_center;
    mutable float italic_shear;
    mutable float whitespace_width;
    mutable float letter_spacing;
    mutable float line_spacing;
    mutable float typing_delay_factor;
    mutable float letter_spacing_factor;
    mutable sf::Color text_color;

    void draw(sf::RenderTarget &target, sf::RenderStates states) const override;

    void ensure_line_break(const printable &printable) const;

    void ensure_updated() const;

    void load_global_text_effects(printable_iterator it);

    void apply_global_text_effects(size_t idx) const;

    void apply_text_effects(const printable &printable) const;

    void set_text_variables() const;

    void remove_global_text_effects(size_t idx) const;

    void remove_text_effects() const;

    void unset_text_variables() const;

    void delay() const;

    inline void new_line() const {
      rect(current_printable).top = y;

      y += line_spacing;
      x = resources->margin_horizontal;
      buffer.push(L'\n');

      rect(current_printable).left = x;
    }

    void apply_mouse_hover(sf::Vector2i cursor);

    void redraw();
};
}

#endif //SFML_GAME_PAGE_H
