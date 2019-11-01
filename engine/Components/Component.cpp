//
// Created by marcin on 9/24/19.
//

#include "Component.hpp"

engine::Component::Component()
    : Identifiable{},
      mDestroyed{},
      mTargetComponent{},
      mName{gen::str("comp-", getUID())},
      mChannel{mName},
      mDependentComponents{} {

}

engine::Component::Component(std::string name)
    : Identifiable{},
      mDestroyed{},
      mTargetComponent{},
      mName{std::move(name)},
      mChannel{mName},
      mDependentComponents{} {

}

void engine::Component::markDestroyed() {
  System::bus().unlisten_all(gen::to_uintptr(this));
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

void engine::Component::notifyChannel(const std::string &channel) const {
  System::bus().notify<std::true_type>(channel, std::integral_constant<bool, true>());
}
