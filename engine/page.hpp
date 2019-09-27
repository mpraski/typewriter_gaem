//
// Created by marcin on 7/19/19.
//

#ifndef SFML_GAME_PAGE_H
#define SFML_GAME_PAGE_H

#ifndef DEBUG
#define DEBUG
#endif

#include <iostream>
#include <vector>
#include <string>
#include <cwctype>
#include <memory>
#include <cmath>
#include <unordered_set>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "Entity.hpp"
#include "AudioSystem.hpp"
#include "printables/printable.hpp"
#include "printables/printable_store.hpp"
#include "printables/dialog.hpp"
#include "story/story.hpp"
#include "translate.hpp"
#include "tweeny.h"

#define pointer(X) (X)->get()
#define rect(X) (X)->get().global_bounds()

namespace engine {
class page : public Entity {
    const constexpr static auto LINE_SHIFT = "line_shift";

    using printable_ref_array = std::vector<std::reference_wrapper<printable>>;
    using printable_ref_array_iterator = printable_ref_array::iterator;
public:
    page(
        SystemPtr sys_ptr,
        audio_system_ptr audio_ptr,
        story_ptr story_ptr
    );

private:
    bool can_advance() const;

    void advance();

    void input();

    void updateSelf(sf::Time dt) final;

    void drawSelf(sf::RenderTarget &target, sf::RenderStates states) const final;

    printable_store store();

    void apply_mouse_hover(const sf::Vector2f &cursor);

    void apply_mouse_click(const sf::Vector2f &cursor);

    void new_line() const;

    bool end_of_page() const;

    void draw_page_outline() const;

    void clear_printable_vertices() const;

private:
    story_ptr story;
    audio_system_ptr audio;
    printable_ref_array printable_refs;
    printable_ref_array_iterator current_printable;
    // SFML buffers
    mutable sf::VertexArray vertices;
    mutable sf::VertexBuffer vertices_buffer;
    mutable sf::VertexArray debug_bounds_vertices;
    mutable sf::VertexArray font_texture_vertices;
    // Control flags
    mutable bool needs_advance;
    mutable bool needs_update;
    mutable bool needs_redraw;
    mutable bool needs_first_preprocess;
    // Text mBounds
    mutable sf::Vector2f next_printable_pos;

    inline auto find_printable(GOId id) {
      return gen::find_if(printable_refs, [id](const auto &ref) {
        return ref.get() == id;
      });
    }
};

template<class ...Ts>
static auto make_page(Ts &&... args) {
  return std::make_unique<page>(std::forward<Ts>(args)...);
}
}

#endif //SFML_GAME_PAGE_H
