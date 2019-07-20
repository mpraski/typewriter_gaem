//
// Created by marcin on 7/19/19.
//

#ifndef SFML_GAME_PAGE_H
#define SFML_GAME_PAGE_H

#include <vector>
#include <string>
#include <cwctype>
#include <memory>
#include <SFML/Graphics/Font.hpp>

namespace engine {
class page {
public:
    page(const std::shared_ptr<sf::Font> &font, unsigned font_size, unsigned page_width);

    void add_paragraph(std::wstring &paragraph);

    const std::vector<std::wstring> &get_paragraphs() const;

private:
    unsigned font_size;
    unsigned page_width;
    std::shared_ptr<sf::Font> font;
    std::vector<std::wstring> paragraphs;

    void wrap_words(std::wstring &paragraph) const;

    static inline void trim_start(std::wstring &s) {
      s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](wchar_t ch) {
        return !std::iswspace(ch);
      }));
    }

    static inline void add_tab(std::wstring &s) {
      s.insert(s.begin(), L'\t');
    }
};
}

#endif //SFML_GAME_PAGE_H
