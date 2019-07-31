//
// Created by marcin on 7/30/19.
//

#ifndef TYPEWRITER_GAEM_DECISION_NODE_H
#define TYPEWRITER_GAEM_DECISION_NODE_H

#include <memory>
#include <vector>

namespace engine {
class decision_node {
public:
    using decision_node_ptr = std::shared_ptr<decision_node>;
private:
    friend class story;

    std::vector<decision_node_ptr> children;
};

using decision_node_ptr = decision_node::decision_node_ptr;
}

#endif //TYPEWRITER_GAEM_DECISION_NODE_H
