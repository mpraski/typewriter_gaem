//
// Created by marcin on 7/20/19.
//

#include <algorithm>
#include "paragraph.h"

engine::paragraph::paragraph(
    const resources_ptr &rptr,
    const std::wstring &c,
    const std::vector<engine::text_effect> &es,
    bool with_tab
) : printable{rptr, c} {
  if (with_tab) {
    add_tab(contents);
  }
  for (const auto &e: es) {
    effects[e.begin].push_back(e);
  }
}

engine::printable *engine::paragraph::clone() const {
  return new paragraph{*this};
}