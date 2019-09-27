//
// Created by marcin on 9/25/19.
//

#include "Collider.hpp"

engine::Collider::Collider()
    : mBounds{},
      mPosition{},
      mVelocity{},
      mMass{},
      mInvertedMass{},
      mEnabled{true},
      mGlobal{true} {
}

sf::FloatRect engine::Collider::localBounds() const {
  return mBounds;
}

sf::FloatRect engine::Collider::globalBounds() const {
  return entity()->getTransform().transformRect(localBounds());
}

engine::Component::Kind engine::Collider::kind() const {
  return Component::Kind::Physics;
}

void engine::Collider::onStart(engine::Entity &entity) {
  auto eventHandler{std::mem_fn(&Collider::handleEvent)};
  if (mGlobal) {
    listen<Event>(eventHandler);
  } else {
    listen<Event>(gen::str("collider", entity.getUID()), eventHandler);
  }
}

void engine::Collider::onEntityUpdate(engine::Entity &entity, sf::Time dt) {

}

void engine::Collider::handleEvent(const Event &event) {

}
