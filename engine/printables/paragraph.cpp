//
// Created by marcin on 7/20/19.
//

#include <algorithm>
#include "paragraph.hpp"

engine::paragraph::paragraph(
    printable::builder &b,
    bool with_tab
) : printable{b} {
  if (with_tab) {
    add_tab(contents);
    offset_effects();
  }
}

engine::printable *engine::paragraph::clone() const {
  return new paragraph{*this};
}