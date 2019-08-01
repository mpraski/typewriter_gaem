//
// Created by marcin on 7/27/19.
//

#include "story.h"

void engine::story::act(engine::action action) {
  switch (action.kind) {
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

        curr_node = (*choice_it).second;

        if (choice_it == std::begin(choices)) {
          store.remove(choices.size() - 1);
        } else {
          store.remove(choices.size());
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