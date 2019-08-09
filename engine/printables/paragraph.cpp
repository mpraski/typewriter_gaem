//
// Created by marcin on 7/20/19.
//

#include <algorithm>
#include "paragraph.hpp"

engine::paragraph::paragraph(
    const system_ptr &rptr,
    const std::wstring &c,
    const std::vector<engine::text_effect> &es,
    bool with_tab
) : printable{rptr, c} {
  for (const auto &e: es) {
    if (e.end >= contents.length()) {
      throw std::runtime_error("Effect lasts longer than its content");
    }
    effects[e.begin].push_back(e);
  }
  if (with_tab) {
    add_tab(contents);
    offset_effects();
  }
}

engine::printable *engine::paragraph::clone() const {
  return new paragraph{*this};
}