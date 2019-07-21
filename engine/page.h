//
// Created by marcin on 7/19/19.
//

#ifndef SFML_GAME_PAGE_H
#define SFML_GAME_PAGE_H

#include <vector>
#include <string>
#include <cwctype>
#include <memory>
#include <SFML/Graphics.hpp>
#include "paragraph.h"
#include "page_config.h"

namespace engine {
class page : public sf::Drawable {
public:
    explicit page(const page_config_ptr &config);

    void add_paragraph(paragraph &&paragraph);

    template<class It>
    void add_paragraphs(It begin, It end) {
      paragraphs.insert(paragraphs.end(),
                        std::make_move_iterator(begin),
                        std::make_move_iterator(end));
    }

    bool text_end() const;

    void advance();

    sf::FloatRect get_local_bounds() const;

private:
    page_config_ptr config;
    // Font config
    std::shared_ptr<sf::Font> font;
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
    // Control flagas
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
    bool is_bold;
    bool is_italic;
    bool is_underlined;
    bool is_strike_through;
    bool is_uppercase;
    float whitespace_width;
    float letter_spacing;
    float line_spacing;
    float typing_delay_factor;

    void draw(sf::RenderTarget &target, sf::RenderStates states) const override;

    void ensure_line_break(const paragraph &paragraph) const;

    void ensure_updated() const;

    void apply_text_effects(paragraph &paragraph) const;

    void remove_text_effects(paragraph &paragraph) const;

    void update_bounds() const;

    void delay() const;

    inline void new_line() const {
      y += line_spacing;
      x = config->margin_horizontal;
    }
};
}

#endif //SFML_GAME_PAGE_H
