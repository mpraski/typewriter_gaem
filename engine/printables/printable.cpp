//
// Created by marcin on 7/24/19.
//

#include "printable.h"

engine::printable::printable(const resources_ptr &rptr, std::wstring &&c)
    : game_state{rptr},
      contents{std::move(c)},
      effects{} {
  if (contents.empty()) {
    throw std::invalid_argument("Contents cannot be empty");
  }

  trim_start(contents);
  trim_end(contents);
  add_end_new_line(contents);
}

std::wstring_view engine::printable::view() const {
  return std::wstring_view{contents};
}

wchar_t engine::printable::operator[](size_t idx) const {
  return contents[idx];
}

size_t engine::printable::length() const {
  return contents.length();
}

void engine::printable::load_effects(size_t idx, engine::printable::back_inserter it) const {
  if (auto k{effects.find(idx)}; k != effects.end())
    std::copy(
        std::cbegin(k->second),
        std::cend(k->second),
        it
    );
}

void engine::printable::break_line_at(size_t idx) {
  if (!std::iswblank(contents[idx])) {
    throw std::runtime_error(general::str("Symbol ", contents[idx], " is not blank"));
  }
  contents[idx] = L'\n';
}

bool engine::printable::interactive() const {
  return false;
}

bool engine::printable::needs_update() const {
  return true;
}

void engine::printable::on_hover_start() {

}

void engine::printable::on_hover_end() {

}

void engine::printable::on_click() {

}