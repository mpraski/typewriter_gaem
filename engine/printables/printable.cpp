//
// Created by marcin on 7/24/19.
//

#include "printable.h"

engine::printable::printable(const resources_ptr &rptr, std::wstring &&c)
    : game_state{rptr},
      id{boost::uuids::random_generator()()},
      contents{std::move(c)},
      effects{} {
  if (contents.empty()) {
    throw std::invalid_argument("Contents cannot be empty");
  }

  trim_start(contents);
  trim_end(contents);
  add_end_new_line(contents);
}

engine::printable_id_t engine::printable::get_id() const {
  return id;
}

const std::wstring &engine::printable::view() const {
  return contents;
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

void engine::printable::inject_line_at(size_t idx) {
  contents.insert(contents.begin() + idx, L'\n');
  offset_effects(idx);
}

bool engine::printable::interactive() const {
  return false;
}

void engine::printable::on_hover_start() {

}

void engine::printable::on_hover_end() {

}

engine::action engine::printable::on_click() {
  return {};
}

void engine::printable::offset_effects(size_t idx, int amount) {
  effect_map new_effects;
  new_effects.reserve(effects.size());
  for (auto &[k, v] : effects) {
    auto new_key{k};
    if (k >= idx) new_key += amount;
    new_effects.insert(std::make_pair(new_key, std::move(v)));
  }
  effects = new_effects;
}