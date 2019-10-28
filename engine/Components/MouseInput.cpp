//
// Created by Marcin Praski on 26.10.19.
//

#include "MouseInput.hpp"

engine::MouseInput::MouseInput() = default;

engine::MouseInput::MouseInput(std::string name)
    : Component(std::move(name)),
      mLastPosition{} {

}

engine::Component::Kind engine::MouseInput::kind() const {
  return Component::Kind::Script;
}

void engine::MouseInput::onStart(engine::Entity &entity) {

}

void engine::MouseInput::onEntityUpdate(engine::Entity &entity, sf::Time dt) {
  const auto &chan{getChannel()};
  auto position{System::instance().mousePosition()};

  if (position != mLastPosition) {
    mLastPosition = position;
    notifyChannel(chan, Event{EventKind::Hover, position});
  }

  if (System::instance().mouseClickAvailable()) {
    notifyChannel(
        chan,
        Event{
            EventKind::Click,
            System::instance().mouseClickPosition()
        }
    );
  }
}

engine::MouseInput::Helper::Helper()
    : mOn{},
      mCapture{true} {
}

void engine::MouseInput::Helper::captureMouseEvents(bool c) {
  mCapture = c;
}
