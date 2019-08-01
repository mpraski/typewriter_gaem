//
// Created by marcin on 7/27/19.
//

#ifndef TYPEWRITER_GAEM_STORY_H
#define TYPEWRITER_GAEM_STORY_H

#include <functional>
#include "../printables/printable.h"
#include "../printables/printable_store.h"
#include "action.h"
#include "decision_node.h"

namespace engine {
class story {
public:
    bool can_progress() const;

    void act(action action);

private:
    printable_store store;
    decision_node_ptr root_node;
    decision_node_ptr curr_node;

    inline auto safe_clone(const printable_ptr &ptr) {
      return printable_ptr{ptr->clone()};
    }
};
}

#endif //TYPEWRITER_GAEM_STORY_H
