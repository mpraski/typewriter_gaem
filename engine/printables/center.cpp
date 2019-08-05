//
// Created by marcin on 8/1/19.
//

#include "center.h"

engine::center::center(
    const resources_ptr &rptr,
    const std::wstring &c
) : printable{rptr, c} {
  std::vector<std::wstring> parts;
  general::wsplit(contents, L'\n', std::back_inserter(parts));
  general::remove_if(parts, [](const auto &p) { return p.empty(); });
  general::for_each(parts, [](auto &p) {
    trim_start(p);
    trim_end(p);
  });

  std::wstringstream wss;

  for (const auto &p : parts) {
    float spaces_width{(resources->effective_page_width() - word_width(p)) / 2};
    size_t spaces{static_cast<size_t>(spaces_width / resources->whitespace_width) -
                  static_cast<size_t>(resources->margin_horizontal / resources->whitespace_width)};
    std::fill_n(std::ostream_iterator<wchar_t, wchar_t>(wss), spaces, L' ');
    wss << p;
    std::fill_n(std::ostream_iterator<wchar_t, wchar_t>(wss), spaces, L' ');
    wss << L'\n';
  }

  contents = wss.str();

  effects[0].push_back(text_effect{text_effect::kind::CENTER, 0, contents.length() - 1});
}

engine::printable *engine::center::clone() const {
  return new center{*this};
}

float engine::center::word_width(const std::wstring &word) const {
  float width{};
  sf::Uint32 curr_char, prev_char{sf::Utf32::decodeWide(word[0])};

  for (auto c : word) {
    curr_char = sf::Utf32::decodeWide(c);

    if (curr_char == L'\r')
      continue;

    width += resources->font->getKerning(prev_char, curr_char, resources->font_size);

    const auto &glyph{resources->font->getGlyph(curr_char, resources->font_size, false)};
    width += glyph.advance + resources->letter_spacing;

    prev_char = curr_char;
  }

  return width;
}