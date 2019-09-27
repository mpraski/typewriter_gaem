//
// Created by marcin on 9/24/19.
//

#include "Component.hpp"

engine::Component::Component()
    : Identifiable{},
      mEntity{},
      mDestroyed{},
      mName{} {

}

engine::Component::~Component() {
  EventBus::instance().unlisten_all(gen::to_uintptr(this));
}

void engine::Component::destroy() {
  mDestroyed = true;
}

void engine::Component::onStart(Entity &entity) {

}

bool engine::Component::destroyed() const noexcept {
  return mDestroyed;
}

const std::string &engine::Component::getName() const {
  return mName;
}

void engine::Component::setName(const std::string &name) {
  mName = name;
}

engine::Entity *engine::Component::entity() const {
  return mEntity;
}