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

namespace engine {
class page : public sf::Drawable {
public:
    page(const std::shared_ptr<sf::Font> &font, size_t font_size, size_t page_width);

    void increment_character();

private:
    size_t font_size;
    size_t page_width;
    size_t current_paragraph;
    size_t current_character;
    std::shared_ptr<sf::Font> font;
    std::vector<paragraph> paragraphs;
    mutable sf::VertexArray vertices;
    mutable sf::VertexBuffer vertices_buffer;
    bool should_flip_new;

    float letter_spacing_factor;
    float line_spacing_factor;
    float whitespace_width;
    float letter_spacing;
    float line_spacing;
    float x;
    float y;
    float min_x;
    float max_x;
    float min_y;
    float max_y;

    void draw(sf::RenderTarget &target, sf::RenderStates states) override;
};
}

#endif //SFML_GAME_PAGE_H
