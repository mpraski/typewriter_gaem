//
// Created by marcin on 9/27/19.
//

#ifndef TYPEWRITER_GAEM_PARAGRAPH_HPP
#define TYPEWRITER_GAEM_PARAGRAPH_HPP

#include "Printable.hpp"

namespace engine {
class Paragraph final : public Printable {
public:
    Paragraph(
        std::wstring contents,
        const std::vector<TextEffect> &textEffects,
        bool withTab = true
    );

private:
    static inline void addTab(std::wstring &s) {
      s.insert(s.begin(), L'\t');
    }
};
}

#endif //TYPEWRITER_GAEM_PARAGRAPH_HPP
