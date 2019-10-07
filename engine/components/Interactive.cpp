//
// Created by marcin on 9/28/19.
//

#include "Interactive.hpp"

engine::Interactive::Interactive()
    : Component{},
      mOn{},
      mInteractive{true},
      mMesh{},
      mChannel{} {

}

void engine::Interactive::onHoverStart() {
  if (mOn) {
    return;
  } else {
    mOn = true;
  }
  notifyChannel(mChannel, Event::HoverStart);
}

void engine::Interactive::onHoverEnd() {
  if (!mOn) {
    return;
  } else {
    mOn = false;
  }
  notifyChannel(mChannel, Event::HoverEnd);
}

void engine::Interactive::onClick() {
  notifyChannel(mChannel, Event::Click);
}

void engine::Interactive::onPress() {
  notifyChannel(mChannel, Event::Press);
}

engine::Component::Kind engine::Interactive::kind() const {
  return Component::Kind::Interactive;
}

void engine::Interactive::onStart(engine::Entity &entity) {
  mMesh = dynamic_cast<Mesh *>(targetComponent());
  if (!mMesh) {
    throw std::runtime_error("Target component is not a mesh");
  }

  mChannel = makeChannel();

  listen<Event>(mChannel, [this](const auto &event) {
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

const std::string &engine::Interactive::getChannel() const {
  return mChannel;
}

sf::FloatRect engine::Interactive::localBounds() const {
  return mMesh->localBounds();
}

sf::FloatRect engine::Interactive::globalBounds() const {
  return entity()->getTransform().transformRect(localBounds());
}

std::string engine::Interactive::makeChannel() {
  return gen::str("action_", targetComponent()->getUID());
}
