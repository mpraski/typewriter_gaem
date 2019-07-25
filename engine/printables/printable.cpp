//
// Created by marcin on 7/24/19.
//

#include "printable.h"

engine::printable::printable(const resources_ptr &rptr, std::wstring &&c)
    : game_state{rptr},
      contents{std::move(c)},
      effects{} {
  assert(!contents.empty());

  trim_start(contents);
  trim_end(contents);
  add_end_space(contents);
}

const std::wstring &engine::printable::str() const {
  return contents;
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