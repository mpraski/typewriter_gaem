//
// Created by marcin on 8/1/19.
//

#ifndef TYPEWRITER_GAEM_PRINTABLE_STORE_H
#define TYPEWRITER_GAEM_PRINTABLE_STORE_H

#include <functional>
#include "printable.h"
#include "paragraph.h"

namespace engine {
class printable_store {
public:
    printable_store();

    printable_store(
        std::function<void(printable_ptr &&)> &&p,
        std::function<void(printable_ptr &&)> &&a,
        std::function<void(size_t)> &&t,
        std::function<void()> &&m,
        std::function<void()> &&s
    );

    std::function<void(printable_ptr &&)> populate;
    std::function<void(printable_ptr &&)> add;
    std::function<void(size_t)> truncate;
    std::function<void()> advance;
    std::function<void()> sync;

    inline static auto safe_clone(const printable_ptr &ptr) {
      return printable_ptr{ptr->clone()};
    }

    static printable_ptr without_effects(const printable_ptr &ptr);

    static printable_ptr without_effects(const printable_ptr &ptr, enum text_effect::kind kind);

    static printable_ptr without_dynamic_effects(const printable_ptr &ptr);

private:
    const constexpr static enum text_effect::kind STATIC_EFFECTS[] = {
        text_effect::kind::BOLD,
        text_effect::kind::ITALIC,
        text_effect::kind::UPPERCASE
    };
};

}

#endif //TYPEWRITER_GAEM_PRINTABLE_STORE_H
