//
// Created by marcin on 9/27/19.
//

#include "Paragraph.hpp"

engine::Paragraph::Paragraph(
    std::wstring contents,
    const std::vector<TextEffect> &textEffects,
    bool withTab
) : Printable(std::move(contents), textEffects) {
  if (withTab) {
    addTab(contents);
    offsetEffects();
  }

  preprocess();
}
