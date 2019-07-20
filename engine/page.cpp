//
// Created by marcin on 7/19/19.
//

#include "page.h"

engine::page::page(const std::shared_ptr<sf::Font> &font,
                   unsigned font_size,
                   unsigned page_width) :
    font{font},
    font_size{font_size},
    page_width{page_width},
    paragraphs{} {

}

void engine::page::add_paragraph(std::wstring &paragraph) {
  wrap_words(paragraph);
  paragraphs.emplace_back(std::move(paragraph));
}

const std::vector<std::wstring> &engine::page::get_paragraphs() const {
  return paragraphs;
}

void engine::page::wrap_words(std::wstring &paragraph) const {
  trim_start(paragraph);
  add_tab(paragraph);

  size_t line_width{0};
  size_t word_width{0};
  size_t last_space{0};

  // To-Do: Should kerning be taken into consideration?
  for (size_t i{0}; i < paragraph.length(); ++i) {
    auto glyph_width{font->getGlyph(paragraph[i], font_size, false).textureRect.width};
    line_width += glyph_width;
    word_width += glyph_width;

    if (std::iswspace(paragraph[i])) {
      if (line_width >= page_width) {
        paragraph[last_space] = L'\n';
        line_width = word_width;
      }

      last_space = i;
      word_width = 0;
    }
  }
}