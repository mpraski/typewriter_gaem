//
// Created by marcin on 9/24/19.
//

#include "Component.hpp"

engine::Component::Component()
    : Identifiable{},
      mDestroyed{},
      mTargetComponent{},
      mName{},
      mChannel{gen::str("comp-", getUID())},
      mDependentComponents{} {

}

void engine::Component::markDestroyed() {
  System::bus().unlisten_all(gen::to_uintptr(this));
  mTargetComponent = nullptr;
  mDestroyed = true;
}

void engine::Component::onStart(Entity &entity) {

}

bool engine::Component::destroyed() const noexcept {
  return mDestroyed;
}

const std::string &engine::Component::getName() const noexcept {
  return mName;
}

void engine::Component::setName(const std::string &name) {
  mName = name;
}

engine::Component *engine::Component::targetComponent() const {
  return mTargetComponent;
}

const std::string &engine::Component::getChannel() const noexcept {
  return mChannel;
}

bool engine::Component::dependent() const {
  return false;
}

void engine::Component::addDependentComponent(sf::Uint64 id) {
  mDependentComponents.push_back(id);
}

const std::vector<sf::Uint64> &engine::Component::getDependentComponents() const noexcept {
    return mDependentComponents;
}