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

namespace engine {
class printable : public game_state {
public:
    using effect_map = std::unordered_map<size_t, std::vector<text_effect>>;
    using back_inserter = std::back_insert_iterator<std::vector<text_effect>>;

    printable(const resources_ptr &rptr, std::wstring &&c);

    // To string
    const std::wstring &str() const;

    // Char of underlying string
    wchar_t operator[](size_t idx) const;

    // Length of underlying string
    const size_t length() const;

    // Push the text effects starting at idx to the back insert iterator
    const void load_effects(size_t idx, back_inserter it) const;

    // Specific callbacks relating to the mouse events inside the printable
    virtual bool needs_update() const;

    virtual void on_hover_start();

    virtual void on_hover_end();

    virtual void on_click();

protected:
    std::wstring contents;
    effect_map effects;

    static inline void trim_start(std::wstring &s) {
      s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](wchar_t ch) {
        return !std::iswspace(ch);
      }));
    }

    static inline void trim_end(std::wstring &s) {
      s.erase(std::find_if(s.rbegin(), s.rend(), [](wchar_t ch) {
        return !std::iswspace(ch);
      }).base(), s.end());
    }

    static inline void add_end_space(std::wstring &s) {
      s.insert(s.end(), L' ');
    }
};
}

#endif //TYPEWRITER_GAEM_PRINTABLE_H
