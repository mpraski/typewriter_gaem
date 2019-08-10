//
// Created by marcin on 7/24/19.
//

#include "dialog.hpp"

engine::dialog::dialog(
    const system_ptr &rptr,
    const std::wstring &person,
    const std::wstring &speech
) : printable{rptr, person + L":\t" + speech, true},
    on{},
    update{},
    effects_on_hover{
        {0,
            {
                text_effect{text_effect::kind::BOLD, 0, person.length() - 1},
                text_effect{text_effect::kind::COLOR, 0, contents.length() - 1}.with_color(sf::Color::Cyan)
            }
        }
    },
    effects_off_hover{
        {0,
            {
                text_effect{text_effect::kind::BOLD, 0, person.length() - 1},
                text_effect{text_effect::kind::COLOR, 0, contents.length() - 1}.with_color(sf::Color::Yellow)
            }
        }
    } {
  if (person.empty()) {
    throw std::invalid_argument("Person cannot be empty");
  }
  if (speech.empty()) {
    throw std::invalid_argument("Speech cannot be empty");
  }

  effects = effects_off_hover;
}

engine::printable *engine::dialog::clone() const {
  return new dialog{*this};
}

void engine::dialog::on_hover_start() {
  if (on) {
    update = false;
    return;
  } else {
    update = true;
    on = true;
  }
  effects = effects_on_hover;
}

void engine::dialog::on_hover_end() {
  if (!on) {
    update = false;
    return;
  } else {
    update = true;
    on = false;
  }
  effects = effects_off_hover;
}

engine::action engine::dialog::on_click() {
  return {action::kind::DIALOG, id};
}