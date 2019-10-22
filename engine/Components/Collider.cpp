//
// Created by marcin on 9/25/19.
//

#include "Collider.hpp"

engine::Collider::Collider()
    : Component{},
      mMesh{},
      mPosition{},
      mVelocity{},
      mMass{},
      mInvertedMass{},
      mEnabled{true},
      mGlobal{true} {
}

sf::FloatRect engine::Collider::localBounds() const {
  return mMesh->localBounds();
}

sf::FloatRect engine::Collider::globalBounds() const {
  return mMesh->globalBounds();
}

engine::Component::Kind engine::Collider::kind() const {
  return Component::Kind::Physics;
}

void engine::Collider::onStart(engine::Entity &entity) {
  mMesh = getTargetComponent<Mesh>();
}

void engine::Collider::onEntityUpdate(engine::Entity &entity, sf::Time dt) {

}

bool engine::Collider::dependent() const {
  return true;
}