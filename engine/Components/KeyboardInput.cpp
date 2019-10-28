//
// Created by Marcin Praski on 25.10.19.
//

#include "KeyboardInput.hpp"

engine::KeyboardInput::KeyboardInput() = default;

engine::KeyboardInput::KeyboardInput(std::string name)
    : Component(std::move(name)) {

}

engine::Component::Kind engine::KeyboardInput::kind() const {
  return Component::Kind::Script;
}

void engine::KeyboardInput::onStart(engine::Entity &entity) {

}

void engine::KeyboardInput::onEntityUpdate(engine::Entity &entity, sf::Time dt) {
  registerEvents(*this);
}

constexpr void engine::KeyboardInput::registerEvents(KeyboardInput &k) {
  const auto &chan{k.getChannel()};
  gen::if_else<
      sf::Keyboard::Key,
      sf::Keyboard::A,
      sf::Keyboard::KeyCount
  >(
      sf::Keyboard::isKeyPressed,
      [&](auto key) {
        k.notifyChannel(chan, key);
      }
  );
}

engine::KeyboardInput::Helper::Helper() : mCapture{true} {

}

void engine::KeyboardInput::Helper::captureKeyboardEvents(bool c) {
  mCapture = c;
}

std::function<void(sf::Keyboard::Key)> engine::KeyboardInput::Helper::getKeyboardListener() {
  return [&](auto key) {
    if (!mCapture) return;
    onKey(key);
  };
}
