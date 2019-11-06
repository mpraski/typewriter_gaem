//
// Created by Marcin Praski on 05.11.19.
//

#include "Parallax.hpp"
#include "../../Entity.hpp"

engine::Component::Kind engine::Parallax::kind() const {
  return Component::Kind::Script;
}

void engine::Parallax::onStart(engine::Entity &entity) {
  listen<PlayerController::Move>(Constants::PlayerChannel, [&entity, this](const auto &msg) {
    notifyChannel(entity.getChannel(), Scroll{msg.x * mModifier, msg.y * mModifier});
  });
}

void engine::Parallax::onEntityUpdate(engine::Entity &entity, sf::Time dt) {

}

engine::Parallax::Parallax(float distance, float maxDistance)
    : mDistance{distance},
      mMaxDistance{maxDistance},
      mModifier{1.f + (maxDistance - distance) / maxDistance} {
  assert(distance >= maxDistance);
}
