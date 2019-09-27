//
// Created by marcin on 9/27/19.
//

#include "Dialog.hpp"

engine::Dialog::Dialog(
    const std::wstring &person,
    const std::wstring &speech
) : Printable{person + L":\t" + speech, {}},
    mOn{},
    mShouldUpdate{},
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

bool engine::Dialog::interactive() {
  return mInteractive;
}

void engine::Dialog::onHoverStart() {
  if (mOn) {
    mShouldUpdate = false;
    return;
  } else {
    mShouldUpdate = true;
    mOn = true;
  }
  mEffects = mEffectsOnHover;
}

void engine::Dialog::onHoverEnd() {
  if (!mOn) {
    mShouldUpdate = false;
    return;
  } else {
    mShouldUpdate = true;
    mOn = false;
  }
  mEffects = mEffectsOffHover;
}

engine::Action engine::Dialog::onClick() {
  return Action{ActionKind::Click, getUID()};
}

engine::Action engine::Dialog::onPress() {
  return Action();
}
