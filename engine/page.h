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
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "game_state.h"
#include "paragraph.h"
#include "sounds.h"

namespace engine {
class page : public game_state, public sf::Drawable, public sf::Transformable {
public:
    page(const resources_ptr &rptr, std::vector<paragraph> &&ps);

    void add_paragraph(paragraph &&paragraph);

    void reset_paragraphs(std::vector<paragraph> &ps);

    bool text_end() const;

    void advance();

    sf::FloatRect get_local_bounds() const;

private:
    sounds snds;
    // Font configuration
    const sf::Font *font;
    unsigned font_size;
    // Character and paragraph cursor
    unsigned current_paragraph;
    unsigned current_character;
    mutable unsigned checked_character;
    // Paragraphs displayed on this page
    mutable std::vector<paragraph> paragraphs;
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

    void ensure_line_break(const paragraph &paragraph) const;

    void ensure_updated() const;

    void apply_text_effects(paragraph &paragraph) const;

    void remove_text_effects() const;

    void update_bounds() const;

    void delay() const;

    inline void new_line() const {
      y += line_spacing;
      x = resources->margin_horizontal;
    }
};
}

#endif //SFML_GAME_PAGE_H
