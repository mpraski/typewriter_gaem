//
// Created by marcin on 7/24/19.
//

#ifndef TYPEWRITER_GAEM_PRINTABLE_HPP
#define TYPEWRITER_GAEM_PRINTABLE_HPP

#include <cstddef>
#include <unordered_map>
#include <vector>
#include "../TextEffect.hpp"
#include "../utilities/general.hpp"
#include "../story/action.hpp"
#include "../Entity.hpp"
#include "../AudioSystem.hpp"

namespace engine {
class printable : public Entity {
    enum class displacement {
        CENTER,
        RIGHT,
        NONE
    };

public:
    using effect_map = std::unordered_map<size_t, std::vector<TextEffect>>;
    using back_inserter = std::back_insert_iterator<effect_map::mapped_type>;
    using effect_it = std::vector<TextEffect>::const_iterator;

    class builder {
    public:
        builder();

        builder &with_system(const SystemPtr &sys_ptr);

        builder &with_audio(const audio_system_ptr &audio_ptr);

        builder &with_contents(std::wstring c);

        builder &with_font_size(unsigned fz);

        builder &with_effects(std::vector<TextEffect> &&ef);

        builder &interactive(bool in);

    private:
        friend class printable;

        SystemPtr _system;
        audio_system_ptr _audio;
        std::wstring _contents;
        std::optional<unsigned> _font_size;
        std::vector<TextEffect> _effects;
        bool _interactive;
    };

    explicit printable(builder &b);

    // To string
    const std::wstring &get_contents() const;

    // Char of underlying string
    inline wchar_t operator[](size_t idx) const {
      return contents[idx];
    }

    // Length of underlying string
    size_t length() const;

    // Length of underlying string without newline
    size_t effective_length() const;

    std::optional<unsigned> get_font_size() const;

    // Can the player interact with this printable (mouse events)
    bool interactive() const;

    // Push the text effects starting range idx to the back insert iterator
    void load_effects(size_t idx);

    // Insert new line character at a blank symbol
    void break_line_at(size_t idx);

    // Insert new line character at a blank symbol
    void inject_line_at(size_t idx);

    // Apply some offset to all effects starting after or at idx
    void offset_effects(size_t idx = 0, int amount = 1);

    printable &with_font_size(unsigned fz);

    // Virtual constructor for deep copying
    virtual printable *clone() const = 0;

    // Specific mCallbacks relating to the mouse events inside the printable
    virtual void on_hover_start();

    virtual void on_hover_end();

    virtual action on_click();

protected:
    friend class printable_store;

    audio_system_ptr audio;
    size_t current_character;
    mutable std::vector<TextEffect> active_effects;
    mutable sf::VertexArray vertices;
    mutable sf::VertexBuffer vertices_buffer;
    mutable unsigned font_size;
    mutable float line_spacing;
    mutable float line_spacing_margin;
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
    // Flags
    mutable bool needs_update;
    mutable bool needs_redraw;
    mutable bool is_current;
    // Text mBounds
    mutable float x;
    mutable float y;
    mutable float min_x;
    mutable float min_y;
    mutable float max_x;
    mutable float max_y;
    // Shit
    std::wstring contents;
    effect_map effects;
    bool is_interactive;

private:

    float measure_text(size_t begin, size_t end);

    void preprocess();

    void ensure_updated();

    void apply_text_effects(size_t idx);

    void remove_text_effects(size_t idx) const;

    void redraw();

    effect_it displacement_effect(enum displacement d) const;

    float displacement_spacing(enum displacement d, float width) const;

    void new_line() const;

    bool end_of_page() const;

    bool end_of_text() const;

    void updateSelf(sf::Time dt) final;

    void drawSelf(sf::RenderTarget &target, sf::RenderStates states) const final;

    static inline void trim_start(std::wstring &s) {
      s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](wchar_t ch) {
        return !std::iswblank(ch);
      }));
    }

    static inline void trim_end(std::wstring &s) {
      s.erase(std::find_if(s.rbegin(), s.rend(), [](wchar_t ch) {
        return !std::iswblank(ch);
      }).base(), s.end());
    }

    static inline void add_end_new_line(std::wstring &s) {
      s.insert(s.end(), L'\n');
    }

    inline auto find_effect(enum TextEffect::Kind kind) const {
      return gen::find_if(active_effects, [&](const auto &e) {
        return e.kind == kind;
      });
    }
};

using printable_ptr = std::unique_ptr<printable>;

}

#endif //TYPEWRITER_GAEM_PRINTABLE_HPP
