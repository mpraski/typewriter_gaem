//
// Created by marcin on 8/1/19.
//

#include "printable_store.hpp"

engine::printable_store::printable_store()
    : populate{[](auto &&) {}},
      add{[](auto &&) {}},
      truncate{[](auto) {}},
      advance{[] {}},
      sync{[] {}} {

}

engine::printable_store::printable_store(
    std::function<void(printable_ptr && )> &&p,
    std::function<void(printable_ptr &&)> &&a,
    std::function<void(size_t)> &&t,
    std::function<void()> &&m,
    std::function<void()> &&s
) : populate{std::move(p)},
    add{std::move(a)},
    truncate{std::move(t)},
    advance{std::move(m)},
    sync{std::move(s)} {

}

engine::printable_ptr engine::printable_store::without_effects(const printable_ptr &ptr) {
  auto cloned{safe_clone(ptr)};
  cloned->is_interactive = false;
  cloned->effects.clear();

  return cloned;
}

engine::printable_ptr engine::printable_store::without_effects(const printable_ptr &ptr, enum TextEffect::Kind kind) {
  auto cloned{safe_clone(ptr)};
  cloned->is_interactive = false;

  for (auto&[pos, effects] : cloned->effects) {
    gen::remove_if(effects, [&](const auto &e) {
      return e.kind == kind;
    });
  }

  return cloned;
}

engine::printable_ptr engine::printable_store::without_dynamic_effects(const printable_ptr &ptr) {
  auto cloned{safe_clone(ptr)};
  cloned->is_interactive = false;

  for (auto&[pos, effects] : cloned->effects) {
    gen::remove_if(effects, [&](const auto &e) {
      return std::find(
          std::begin(STATIC_EFFECTS),
          std::end(STATIC_EFFECTS),
          e.kind
      ) == std::end(STATIC_EFFECTS);
    });
  }

  return cloned;
}