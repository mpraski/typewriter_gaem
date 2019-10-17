//
// Created by marcin on 9/24/19.
//

#include "Component.hpp"

engine::Component::Component()
    : Identifiable{},
      mEntity{},
      mTargetComponent{},
      mDestroyed{},
      mName{},
      mChannel{gen::str("comp-", getUID())},
      mAttachedComponents{} {

}

void engine::Component::markDestroyed() {
  EventBus::instance().unlisten_all(gen::to_uintptr(this));
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

engine::Entity *engine::Component::entity() const {
  return mEntity;
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

engine::Component::Component(const engine::Component &comp)
    : Identifiable{},
      mEntity{},
      mTargetComponent{},
      mDestroyed{},
      mName{comp.mName},
      mChannel{gen::str("comp-", getUID())},
      mAttachedComponents{} {
  assert(!comp.mDestroyed);
}