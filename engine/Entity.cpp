//
// Created by marcin on 8/15/19.
//

#include "Entity.hpp"

engine::Entity::Entity()
    : Identifiable{},
      mParent{nullptr},
      mDestroyed{},
      mChildren{},
      mComponents{},
      mDrawables{},
      mClock{},
      mSinceLastUpdate{sf::Time::Zero} {

}

void engine::Entity::addChild(Ptr entity) {
  entity->mParent = this;
  mChildren.push_back(std::move(entity));
}

engine::EntityPtr engine::Entity::removeChild(const engine::Entity &entity) {
  auto child{gen::find_if(mChildren, [&](const auto &ptr) { return ptr.get() == &entity; })};
  if (child == std::end(mChildren)) {
    throw std::runtime_error("Child scene node not present");
  }

  auto evicted{std::move(*child)};
  evicted->mParent = nullptr;
  mChildren.erase(child);
  return evicted;
}

sf::Transform engine::Entity::globalTransform() const {
  auto transform{sf::Transform::Identity};
  for (const auto *ptr{this}; ptr != nullptr; ptr = ptr->mParent) {
    transform *= ptr->getTransform();
  }
  return transform;
}

sf::Vector2f engine::Entity::globalPosition() const {
  return globalTransform() * sf::Vector2f{};
}

bool engine::Entity::hasElapsed(sf::Time dt) {
  if (mSinceLastUpdate > dt) {
    mSinceLastUpdate = sf::Time::Zero;
    return true;
  }

  return false;
}

bool engine::Entity::root() const noexcept {
  return !mParent;
}

bool engine::Entity::destroyed() const noexcept {
  return mDestroyed;
}

void engine::Entity::update(sf::Time dt) {
  EventBus::instance().deliver();

  updateSelf(dt);
  updateChildren(dt);
}

void engine::Entity::destroy() {
  for (auto &c : mComponents) {
    c->destroy();
  }
  mDestroyed = true;
}

void engine::Entity::updateSelf(sf::Time dt) {
  for (auto it{std::begin(mComponents)}; it != std::end(mComponents);) {
    (*it)->onEntityUpdate(*this, dt);
    if ((*it)->destroyed()) {
      if ((*it)->kind() == Component::Kind::Mesh) {
        auto r{gen::find(mDrawables, dynamic_cast<sf::Drawable *>(it->get()))};
        if (r != std::end(mDrawables)) mDrawables.erase(r);
      }
      it = mComponents.erase(it);
    } else {
      ++it;
    }
  }

  if (mComponents.empty()) {
    destroy();
  }
}

void engine::Entity::drawSelf(sf::RenderTarget &target, sf::RenderStates states) const {
  for (const auto &d : mDrawables) {
    target.draw(*d, states);
  }
}

void engine::Entity::updateChildren(sf::Time dt) {
  for (auto it{std::begin(mChildren)}; it != std::end(mChildren);) {
    (*it)->update(dt);
    if ((*it)->destroyed()) {
      it = mChildren.erase(it);
    } else {
      ++it;
    }
  }
}

void engine::Entity::drawChildren(sf::RenderTarget &target, sf::RenderStates states) const {
  for (const auto &c : mChildren) {
    target.draw(*c, states);
  }
}

void engine::Entity::draw(sf::RenderTarget &target, sf::RenderStates states) const {
  states.transform *= getTransform();

  drawSelf(target, states);
  drawChildren(target, states);

  sf::Time elapsed{mClock.restart()};
  mSinceLastUpdate += elapsed;
}