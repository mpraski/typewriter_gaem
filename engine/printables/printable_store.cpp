//
// Created by marcin on 8/1/19.
//

#include "printable_store.h"

engine::printable_store::printable_store()
    : populate{[](auto &&) {}},
      add{[](auto &&) {}},
      truncate{[](auto) {}} {

}

engine::printable_store::printable_store(
    std::function<void(printable_ptr && )> &&p,
    std::function<void(printable_ptr &&)> &&a,
    std::function<void(size_t)> &&t
) : populate{std::move(p)},
    add{std::move(a)},
    truncate{std::move(t)} {

}