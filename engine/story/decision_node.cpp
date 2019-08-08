//
// Created by marcin on 7/30/19.
//

#include "decision_node.hpp"

engine::decision_node::decision_node(
    const std::vector<printable *> &cnts,
    const std::vector<std::pair<printable *, decision_node *>> &cshs
) : contents{}, choices{} {
  for (auto p : cnts) {
    contents.emplace_back(p);
  }
  for (auto[p, d] : cshs) {
    choices.emplace_back(p, d);
  }
}