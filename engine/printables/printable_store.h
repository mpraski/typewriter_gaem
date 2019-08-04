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
        std::function<void(int)> &&m,
        std::function<void()> &&s
    );

    std::function<void(printable_ptr &&)> populate;
    std::function<void(printable_ptr &&)> add;
    std::function<void(size_t)> truncate;
    std::function<void(int)> advance;
    std::function<void()> sync;

    inline static auto safe_clone(const printable_ptr &ptr) {
      return printable_ptr{ptr->clone()};
    }

    inline static auto without_effects(const printable_ptr &ptr) {
      return printable_ptr{
          new paragraph{ptr->get_resources(), ptr->get_contents(), {}, false}
      };
    }

    inline static auto without_effects(const printable_ptr &ptr, enum text_effect::kind kind) {
      auto cloned{safe_clone(ptr)};
      cloned->is_interactive = false;

      for (auto&[pos, effects] : cloned->effects) {
        effects.erase(
            std::remove_if(
                std::begin(effects),
                std::end(effects),
                [&](const auto &e) {
                  return e.kind == kind;
                }
            ),
            std::end(effects)
        );
      }

      return cloned;
    }
};

}

#endif //TYPEWRITER_GAEM_PRINTABLE_STORE_H
