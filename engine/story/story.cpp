//
// Created by marcin on 7/27/19.
//

#include "story.h"

engine::story::story(const decision_node_ptr &ptr)
    : store{},
      root_node{ptr},
      curr_node{ptr} {

}

void engine::story::set_store(printable_store &&s) {
  store = std::move(s);
}

void engine::story::act(engine::action action) {
  switch (action.kind) {
    case action::kind::INITIAL:
      for (const auto &p : curr_node->contents) {
        store.populate(safe_clone(p));
      }

      for (const auto&[p, d] : curr_node->choices) {
        store.populate(safe_clone(p));
      }
      break;
    case action::kind::DIALOG:
      const auto &choices{curr_node->choices};
      if (auto choice_it{
            std::find_if(
                std::begin(choices),
                std::end(choices),
                [&](const auto &p) {
                  return p.first->get_id() == action.pid;
                }
            )
        }; choice_it != std::end(choices)) {

        curr_node = choice_it->second;

        if (choice_it == std::begin(choices)) {
          store.truncate(choices.size() - 1);
        } else {
          store.truncate(choices.size());
          // copy the selected printable and add it
          store.add(safe_clone(choice_it->first));
        }
        // add the contents of curr_node
        for (const auto &p : curr_node->contents) {
          store.add(safe_clone(p));
        }
      } else {
        throw std::runtime_error("Selected printable is not a choice");
      }
      break;
  }
}