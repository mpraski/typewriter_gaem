//
// Created by marcin on 7/30/19.
//

#ifndef TYPEWRITER_GAEM_DECISION_NODE_H
#define TYPEWRITER_GAEM_DECISION_NODE_H

#include <memory>
#include <vector>
#include "../printables/printable.h"

namespace engine {
class decision_node {
public:
    using decision_node_ptr = std::shared_ptr<decision_node>;

    decision_node(
        const std::vector<printable *> &cnts,
        const std::vector<std::pair<printable *, decision_node *>>& cshs
    );

private:
    friend class story;

    std::vector<printable_ptr> contents;
    std::vector<std::pair<printable_ptr, decision_node_ptr>> choices;
};

using decision_node_ptr = decision_node::decision_node_ptr;
}

#endif //TYPEWRITER_GAEM_DECISION_NODE_H
