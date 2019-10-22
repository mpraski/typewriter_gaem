//
// Created by marcin on 9/28/19.
//

#include "Interactive.hpp"

engine::Interactive::Interactive()
    : Component{},
      mOn{},
      mInteractive{true},
      mMesh{},
      mInterface{} {

}

void engine::Interactive::onHoverStart() {
  if (mOn) {
    return;
  } else {
    mOn = true;
  }
  mInterface->onHoverStart();
}

void engine::Interactive::onHoverEnd() {
  if (!mOn) {
    return;
  } else {
    mOn = false;
  }
  mInterface->onHoverEnd();
}

void engine::Interactive::onClick() {
  mInterface->onClick();
}

engine::Component::Kind engine::Interactive::kind() const {
  return Component::Kind::Interactive;
}

void engine::Interactive::onStart(engine::Entity &entity) {
    mMesh = getTargetComponent<Mesh>();
    mInterface = getTargetComponent<Interface>();

  listen<Event>(getChannel(), [this](const auto &event) {
    switch (event) {
      case Event::Enable:
        mInteractive = true;
        break;
      case Event::Disable:
        mInteractive = false;
        break;
      default:
        break;
    }
  });
}

void engine::Interactive::onEntityUpdate(engine::Entity &entity, sf::Time dt) {

}

bool engine::Interactive::interactive() {
  return mInteractive;
}

sf::FloatRect engine::Interactive::localBounds() const {
  return mMesh->localBounds();
}

sf::FloatRect engine::Interactive::globalBounds() const {
  return mMesh->globalBounds();
}

bool engine::Interactive::dependent() const {
  return true;
}