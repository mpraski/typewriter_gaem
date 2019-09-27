//
// Created by marcin on 8/1/19.
//

#ifndef TYPEWRITER_GAEM_PRINTABLE_STORE_HPP
#define TYPEWRITER_GAEM_PRINTABLE_STORE_HPP

#include <functional>
#include "printable.hpp"
#include "paragraph.hpp"
#include "../utilities/General.hpp"

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

    static printable_ptr without_effects(const printable_ptr &ptr, enum TextEffect::Kind kind);

    static printable_ptr without_dynamic_effects(const printable_ptr &ptr);

private:
    const constexpr static enum TextEffect::Kind STATIC_EFFECTS[] = {
        TextEffect::Kind::BOLD,
        TextEffect::Kind::ITALIC,
        TextEffect::Kind::UPPERCASE
    };
};
}

#endif //TYPEWRITER_GAEM_PRINTABLE_STORE_HPP
