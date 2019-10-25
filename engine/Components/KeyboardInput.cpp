//
// Created by Marcin Praski on 25.10.19.
//

#include "KeyboardInput.hpp"

constexpr void engine::KeyboardInput::registerEvents(KeyboardInput &k) {
  const auto &chan{k.getChannel()};
  gen::enum_for<
      sf::Keyboard::Key,
      sf::Keyboard::A,
      sf::Keyboard::KeyCount
  >(
      [&](auto key) {
        k.notifyChannel(chan, key);
      }
  );
}

engine::KeyboardInput::KeyboardInput() : Component() {

}

engine::Component::Kind engine::KeyboardInput::kind() const {
  return Component::Kind::Script;
}

void engine::KeyboardInput::onStart(engine::Entity &entity) {

}

void engine::KeyboardInput::onEntityUpdate(engine::Entity &entity, sf::Time dt) {
  registerEvents(*this);
}
