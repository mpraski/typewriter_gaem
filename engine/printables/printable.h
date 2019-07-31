//
// Created by marcin on 7/24/19.
//

#ifndef TYPEWRITER_GAEM_PRINTABLE_H
#define TYPEWRITER_GAEM_PRINTABLE_H

#include <iostream>
#include <cstddef>
#include <assert.h>
#include <unordered_map>
#include <vector>
#include "../text_effect.h"
#include "../game_state.h"
#include "../utilities/general.h"

namespace engine {
class printable : public game_state {
public:
    using effect_map = std::unordered_map<size_t, std::vector<text_effect>>;
    using back_inserter = std::back_insert_iterator<effect_map::mapped_type>;

    printable(const resources_ptr &rptr, std::wstring &&c);

    // To string
    std::wstring_view view() const;

    // Char of underlying string
    wchar_t operator[](size_t idx) const;

    // Length of underlying string
    size_t length() const;

    // Push the text effects starting range idx to the back insert iterator
    void load_effects(size_t idx, back_inserter it) const;

    // Insert new line character at a blank symbol
    void break_line_at(size_t idx);

    // Insert new line character at a blank symbol
    void inject_line_at(size_t idx);

    // Apply some offset to all effects starting after or at idx
    void offset_effects(size_t idx, int amount = 1);

    // Specific callbacks relating to the mouse events inside the printable
    virtual bool interactive() const;

    virtual bool needs_update() const;

    virtual void on_hover_start();

    virtual void on_hover_end();

    virtual void on_click();

protected:
    std::wstring contents;
    effect_map effects;

private:
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
};

using printable_ptr = std::unique_ptr<printable>;
}

#endif //TYPEWRITER_GAEM_PRINTABLE_H
