//
// Created by marcin on 7/20/19.
//

#ifndef TYPEWRITER_GAEM_PARAGRAPH_HPP
#define TYPEWRITER_GAEM_PARAGRAPH_HPP

#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <algorithm>
#include <cwctype>
#include "printable.hpp"
#include "../TextEffect.hpp"

namespace engine {
class paragraph : public printable {
public:
    paragraph(
        printable::builder &b,
        bool with_tab = true
    );

    printable *clone() const override;

private:
    static inline void add_tab(std::wstring &s) {
      s.insert(s.begin(), L'\t');
    }
};
}

#endif //TYPEWRITER_GAEM_PARAGRAPH_HPP
