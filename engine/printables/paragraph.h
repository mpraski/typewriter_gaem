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
    static inline void add_tab(std::wstring &s) {
      s.insert(s.begin(), L'\t');
    }
};
}

#endif //TYPEWRITER_GAEM_PARAGRAPH_H
