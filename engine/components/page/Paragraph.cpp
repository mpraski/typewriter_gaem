//
// Created by marcin on 9/27/19.
//

#include "Paragraph.hpp"

bool engine::Paragraph::interactive() {
  return false;
}

void engine::Paragraph::onHoverStart() {

}

void engine::Paragraph::onHoverEnd() {

}

engine::Action engine::Paragraph::onClick() {
  return Action();
}

engine::Action engine::Paragraph::onPress() {
  return Action();
}

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
