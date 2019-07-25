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
#include "printable.h"
#include "../text_effect.h"
#include "../utilities/converters.h"

namespace engine {
class paragraph : public printable {
public:
    paragraph(
        const resources_ptr &rptr,
        std::wstring &&c,
        std::vector<engine::text_effect> &&es
    );

private:
    inline void prepare(std::wstring &s) {
      add_tab(s);
    }

    static inline void trim_inside(std::wstring &s) {
      s.erase(std::unique(s.begin(), s.end(), [](wchar_t l, wchar_t r) {
        return (l == r) && std::iswspace(l);
      }), s.end());
    }

    static inline void add_tab(std::wstring &s) {
      s.insert(s.begin(), L'\t');
    }
};
}

#endif //TYPEWRITER_GAEM_PARAGRAPH_H
