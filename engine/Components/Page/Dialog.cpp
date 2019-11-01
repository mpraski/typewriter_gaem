//
// Created by marcin on 9/27/19.
//

#include "Dialog.hpp"
#include "../../Entity.hpp"
#include "../../Engine.hpp"

engine::Dialog::Dialog(
    const std::wstring &person,
    const std::wstring &speech
) : Printable{person + L":\t" + speech, {}},
    mEffectsOnHover{
        {0,
            {
                TextEffect{TextEffect::Kind::BOLD, 0, person.length() - 1},
                TextEffect{TextEffect::Kind::COLOR, 0, mContents.length() - 1}.withColor(sf::Color::Cyan)
            }
        }
    },
    mEffectsOffHover{
        {0,
            {
                TextEffect{TextEffect::Kind::BOLD, 0, person.length() - 1},
                TextEffect{TextEffect::Kind::COLOR, 0, mContents.length() - 1}.withColor(sf::Color::Yellow)
            }
        }
    } {
  mEffects = mEffectsOffHover;

  preprocess();
}

void engine::Dialog::onStart(engine::Entity &entity) {
  Printable::onStart(entity);

  listen<MouseInput::Event>(Constants::MouseChannel, getMouseListener(*this));

  listen("page_scroll_begin", [this](const auto &msg) {
    captureMouseEvents(false);
  });

  listen("page_scroll_end", [this](const auto &msg) {
    captureMouseEvents(true);
  });
}

void engine::Dialog::onHoverStart() {
  mEffects = mEffectsOnHover;
}

void engine::Dialog::onHoverEnd() {
  mEffects = mEffectsOffHover;
}

void engine::Dialog::onClick() {
  notifyChannel("dialog_selection", getUID());
}
