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
    explicit story(const decision_node_ptr &ptr);

    void set_store(printable_store &&s);

    void init();

    void act(action action);

private:
    printable_store store;
    decision_node_ptr root_node;
    decision_node_ptr curr_node;

    void add_printables(const decision_node_ptr &ptr);
};

using story_ptr = std::shared_ptr<story>;
}

#endif //TYPEWRITER_GAEM_STORY_H
