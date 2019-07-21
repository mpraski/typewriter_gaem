//
// Created by marcin on 7/20/19.
//

#include <algorithm>
#include "paragraph.h"

engine::paragraph::paragraph(std::wstring &&c)
    : contents{std::move(c)},
      effects{},
      active_effects{} {
  trim_start(contents);
  trim_end(contents);
  add_tab(contents);
  add_end_space(contents);
};

engine::paragraph::paragraph(const char *c)
    : contents{utils::to_wstr(c)},
      effects{},
      active_effects{} {
  trim_start(contents);
  trim_end(contents);
  add_tab(contents);
  add_end_space(contents);
};

const size_t engine::paragraph::length() const {
  return contents.length();
}

wchar_t engine::paragraph::operator[](size_t idx) const {
  return contents[idx];
}

engine::paragraph::wstr_iterator engine::paragraph::begin() const {
  return std::cbegin(contents);
}

engine::paragraph::wstr_iterator engine::paragraph::end() const {
  return std::cend(contents);
}

const std::wstring &engine::paragraph::get_contents() const {
  return contents;
}

const std::vector<engine::text_effect> &engine::paragraph::get_active_effects() const {
  return active_effects;
}

void engine::paragraph::add_starting_effects(size_t pos) {
  auto &effects_for_pos{effects[pos]};
  active_effects.insert(
      std::end(active_effects),
      std::begin(effects_for_pos),
      std::end(effects_for_pos)
  );
}

void engine::paragraph::remove_ending_effects(size_t pos) {
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