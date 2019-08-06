//
// Created by marcin on 7/19/19.
//

#ifndef SFML_GAME_PAGE_H
#define SFML_GAME_PAGE_H

#define DEBUG

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
#include "audio_system.h"
#include "printables/printable.h"
#include "printables/printable_store.h"
#include "printables/dialog.h"
#include "utilities/text_buffer.h"
#include "story/story.h"

#define pointer(IT) (IT)->first
#define rect(IT) (IT)->second

namespace engine {
class page : public game_state, public sf::Drawable, public sf::Transformable {
    using printable_array = std::vector<std::pair<printable_ptr, sf::FloatRect>>;
    using printable_iterator = printable_array::iterator;
    using effect_array = std::vector<text_effect>;
public:
    page(const resources_ptr &rptr, const story_ptr &sptr);

    bool text_end() const;

    void advance();

    void input();

private:
    enum class displacement {
        CENTER,
        RIGHT,
        NONE
    };

    story_ptr story;
    audio_system audio;
    // Core data
    mutable printable_array printables;
    mutable effect_array active_effects;
    printable_iterator current_printable;
    size_t current_character;
    // SFML buffers
    mutable sf::VertexArray vertices;
    mutable sf::VertexBuffer vertices_buffer;
    mutable sf::VertexArray debug_bounds_vertices;
    mutable sf::VertexArray font_texture_vertices;
    mutable sf::FloatRect bounds;
    // Control flags
    mutable bool end_of_text;
    mutable bool needs_update;
    mutable bool needs_redraw;
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
    mutable displacement displacement;
    mutable size_t displacement_mode_end;
    mutable size_t displacement_mode_end_prev;
    mutable float italic_shear;
    mutable float typing_delay_factor;
    mutable float letter_spacing_factor;
    mutable sf::Color text_color;
    mutable const sf::Texture *text_texture;

    void draw(sf::RenderTarget &target, sf::RenderStates states) const override;

    sf::FloatRect local_bounds() const;

    sf::FloatRect global_bounds() const;

    printable_store store();

    float measure_text(const printable &printable, size_t begin, size_t end) const;

    void preprocess(printable &printable);

    void ensure_updated() const;

    void apply_text_effects(const printable &printable, size_t idx) const;

    void remove_text_effects(size_t idx) const;

    void delay() const;

    void apply_mouse_position(const sf::Vector2f &cursor);

    void apply_mouse_click_position(const sf::Vector2f &cursor);

    void redraw();

    effect_array::const_iterator displacement_effect(enum displacement d) const;

    float displacement_spacing(enum displacement d, float width) const;

    inline void new_line() const {
      y += resources->line_spacing;
      x = resources->margin_horizontal;
    }

    inline auto find_printable(printable_id_t id) const {
      return general::find(printables, [&](const auto &p) {
        return p.first->get_id() == id;
      });
    }

    inline auto find_effect(enum text_effect::kind kind) const {
      return general::find(active_effects, [&](const auto &e) {
        return e.kind == kind;
      });
    }
};
}

#endif //SFML_GAME_PAGE_H
