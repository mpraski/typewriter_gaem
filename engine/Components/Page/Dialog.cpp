//
// Created by marcin on 9/27/19.
//

#include "Dialog.hpp"

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

  auto interactive = std::make_unique<Interactive>();
  auto chan = interactive->getChannel();

  addComponent(std::move(interactive), this);

  listen("page_scroll_begin", [this, chan](const auto &msg) {
    notifyChannel(chan, Interactive::Event::Disable);
  });

  listen("page_scroll_end", [this, chan](const auto &msg) {
    notifyChannel(chan, Interactive::Event::Enable);
  });
}

void engine::Dialog::onHoverStart() {
  mEffects = mEffectsOnHover;
}

void engine::Dialog::onHoverEnd() {
  mEffects = mEffectsOffHover;
}

void engine::Dialog::onClick() {

}
