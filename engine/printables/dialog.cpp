//
// Created by marcin on 7/24/19.
//

#include "dialog.h"

engine::dialog::dialog(
    const resources_ptr &rptr,
    const std::wstring &person,
    const std::wstring &speech
) : printable{rptr, person + L":\t" + speech},
    on{},
    update{},
    effects_on_hover{},
    effects_off_hover{} {
  effects_off_hover[0].push_back(text_effect{text_effect::kind::BOLD, 0, person.length()});
  effects_off_hover[0].push_back(
      text_effect{text_effect::kind::COLOR, 0, contents.length()}.with_color(sf::Color::Yellow));

  effects_on_hover[0].push_back(text_effect{text_effect::kind::BOLD, 0, person.length()});
  effects_on_hover[0].push_back(
      text_effect{text_effect::kind::COLOR, 0, contents.length()}.with_color(sf::Color::Cyan));

  effects = effects_off_hover;
}

bool engine::dialog::interactive() const {
  return true;
}

bool engine::dialog::needs_update() const {
  return update;
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

void engine::dialog::on_click() {

}