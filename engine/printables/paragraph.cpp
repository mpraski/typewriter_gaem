//
// Created by marcin on 7/20/19.
//

#include <algorithm>
#include "paragraph.h"

engine::paragraph::paragraph(
    const resources_ptr &rptr,
    std::wstring &&c,
    std::vector<engine::text_effect> &&es
) : printable{rptr, std::move(c)} {
  add_tab(contents);
  for (const auto &e: es) {
    effects[e.begin].push_back(e);
  }
}