//
// Created by marcin on 7/20/19.
//

#ifndef TYPEWRITER_GAEM_PARAGRAPH_H
#define TYPEWRITER_GAEM_PARAGRAPH_H


#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <algorithm>
#include <cwctype>
#include "text_effect.h"
#include "utils.h"

namespace engine {
class paragraph {
    using wstr_iterator = std::wstring::const_iterator;
public:
    explicit paragraph(std::wstring &&c);

    explicit paragraph(const char *c);

    const size_t length() const;

    wchar_t operator[](size_t idx) const;

    wstr_iterator begin() const;

    wstr_iterator end() const;

    const std::wstring &get_contents() const;

    const std::vector<text_effect> &get_active_effects() const;

    void add_starting_effects(size_t pos);

    void remove_ending_effects(size_t pos);

private:
    std::wstring contents;
    std::unordered_map<size_t, std::unordered_set<text_effect>> effects;
    std::vector<text_effect> active_effects;

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

    static inline void add_tab(std::wstring &s) {
      s.insert(s.begin(), L'\t');
    }

    static inline void add_end_space(std::wstring &s) {
      s.insert(s.end(), L' ');
    }
};
}

#endif //TYPEWRITER_GAEM_PARAGRAPH_H
