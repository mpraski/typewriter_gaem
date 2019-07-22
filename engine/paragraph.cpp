//
// Created by marcin on 7/20/19.
//

#include <algorithm>
#include "paragraph.h"

engine::paragraph::paragraph(std::wstring &&c)
    : contents{std::move(c)},
      effects{},
      active_effects{} {
  prepare(contents);
};

engine::paragraph::paragraph(std::wstring &&c, const std::vector<text_effect> &es)
    : contents{std::move(c)},
      effects{},
      active_effects{} {
  prepare(contents);
  for (const auto &e : es) {
    effects[e.begin].insert(e);
  }
};

const size_t engine::paragraph::length() const {
  return contents.length();
}

engine::paragraph::iter engine::paragraph::begin() const {
  return std::cbegin(contents);
}

engine::paragraph::iter engine::paragraph::end() const {
  return std::cend(contents);
}

const std::vector<engine::text_effect> &engine::paragraph::get_active_effects() const {
  return active_effects;
}

void engine::paragraph::add_starting_effects(unsigned pos) {
  auto &effects_for_pos{effects[pos]};
  for (const auto &e : effects_for_pos) {
    if (std::find(std::begin(active_effects), std::end(active_effects), e)
        == std::end(active_effects)) {
      active_effects.push_back(e);
    }
  }
}

void engine::paragraph::remove_ending_effects(unsigned pos) {
  active_effects.erase(
      std::remove_if(
          active_effects.begin(),
          active_effects.end(),
          [pos](const auto &e) {
            return pos == e.end;
          }
      ),
      active_effects.end()
  );
}