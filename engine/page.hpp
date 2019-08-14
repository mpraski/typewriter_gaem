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
#include "game_state.hpp"
#include "game_object.hpp"
#include "audio_system.hpp"
#include "printables/printable.hpp"
#include "printables/printable_store.hpp"
#include "printables/dialog.hpp"
#include "story/story.hpp"
#include "animation/translate.hpp"
#include "tweeny.h"

#define pointer(IT) (IT)->first
#define rect(IT) (IT)->second

namespace engine {
class page : public game_object, public sf::Drawable, public sf::Transformable {
    using printable_array = std::vector<std::pair<printable_ptr, sf::FloatRect>>;
    using printable_iterator = printable_array::iterator;
    using effect_array = std::vector<text_effect>;
    using effect_it = effect_array::const_iterator;
    using check_visibility = std::function<bool(const sf::FloatRect &)>;
public:
    page(const system_ptr &rptr, const story_ptr &sptr, check_visibility cv);

    bool can_advance() const;

    void advance();

    void input();

private:
    enum class displacement {
        CENTER,
        RIGHT,
        NONE
    };

    check_visibility visible;
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
    mutable translate_vertical line_shift_animation;
    // Control flags
    mutable bool needs_advance;
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

    void apply_mouse_hover(const sf::Vector2f &cursor);

    void apply_mouse_click(const sf::Vector2f &cursor);

    void redraw();

    effect_it displacement_effect(enum displacement d) const;

    float displacement_spacing(enum displacement d, float width) const;

    void new_line() const;

    bool end_of_page() const;

    bool end_of_text() const;

    void draw_printable_outline(printable_iterator it) const;

    void draw_page_outline() const;

    inline auto find_printable(id_t id) const {
      return gen::find(printables, [&](const auto &p) {
        return *p.first == id;
      });
    }

    inline auto find_effect(enum text_effect::kind kind) const {
      return gen::find(active_effects, [&](const auto &e) {
        return e.kind == kind;
      });
    }
};
}

#endif //SFML_GAME_PAGE_H
