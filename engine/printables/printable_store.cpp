//
// Created by marcin on 8/1/19.
//

#include "printable_store.h"

engine::printable_store::printable_store()
    : populate{[](auto &&) {}},
      add{[](auto &&) {}},
      truncate{[](auto) {}},
      advance{[](auto) {}},
      sync{[] {}} {

}

engine::printable_store::printable_store(
    std::function<void(printable_ptr && )> &&p,
    std::function<void(printable_ptr &&)> &&a,
    std::function<void(size_t)> &&t,
    std::function<void(int)> &&m,
    std::function<void()> &&s
) : populate{std::move(p)},
    add{std::move(a)},
    truncate{std::move(t)},
    advance{std::move(m)},
    sync{std::move(s)} {

}