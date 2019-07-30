//
// Created by marcin on 7/27/19.
//

#ifndef TYPEWRITER_GAEM_STORY_H
#define TYPEWRITER_GAEM_STORY_H

#include <functional>
#include "../printables/printable.h"
#include "action.h"

namespace engine {
class story {
public:
    bool can_progress() const;

    void update_printables(
        const std::function<void(printable_ptr &&printable)> &adder,
        const std::function<void(size_t last_printables)> &remover
    );

    void act(action action);
};
}

#endif //TYPEWRITER_GAEM_STORY_H
