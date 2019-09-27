//
// Created by marcin on 7/24/19.
//

#include "dialog.hpp"

engine::dialog::dialog(
    printable::builder &b,
    const std::wstring &person,
    const std::wstring &speech
) : printable{
    b.interactive(true)
        .with_contents(person + L":\t" + speech)
},
    on{},
    update{},
    effects_on_hover{
        {0,
            {
                TextEffect{TextEffect::Kind::BOLD, 0, person.length() - 1},
                TextEffect{TextEffect::Kind::COLOR, 0, contents.length() - 1}.withColor(sf::Color::Cyan)
            }
        }
    },
    effects_off_hover{
        {0,
            {
                TextEffect{TextEffect::Kind::BOLD, 0, person.length() - 1},
                TextEffect{TextEffect::Kind::COLOR, 0, contents.length() - 1}.withColor(sf::Color::Yellow)
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
  return {action::kind::DIALOG, mUID};
}