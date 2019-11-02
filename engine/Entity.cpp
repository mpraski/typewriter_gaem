//
// Created by marcin on 8/15/19.
//

#include "Entity.hpp"

engine::Entity::Entity()
    : Identifiable{},
      mDestroyed{},
      mChannel{gen::str("ent-", getUID())},
      mParent{},
      mChildren{},
      mComponents{},
      mQueuedComponents{},
      mDeferred{},
      mMeshes{},
      mComponentCache{},
      mClock{},
      mSinceLastUpdate{sf::Time::Zero} {

}

engine::Entity::~Entity() {
  for (auto &c : mComponents) {
    c->markDestroyed();
  }
  mDestroyed = true;
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

bool engine::Entity::destroyed() const noexcept {
  return mDestroyed;
}

void engine::Entity::update(sf::Time dt) {
  System::bus().deliver();

  updateSelf(dt);
  updateChildren(dt);
}

void engine::Entity::updateSelf(sf::Time dt) {
  for (auto it{std::begin(mComponents)}; it != std::end(mComponents);) {
    if ((*it)->destroyed()) {
      for (const auto &depUID : (*it)->getDependentComponents()) {
        if (auto *depComp{getComponent(depUID)}; depComp) {
          depComp->markDestroyed();
        }
      }
      if ((*it)->dependent()) {
        gen::remove_if(
            (*it)->mTargetComponent->mDependentComponents,
            [u = (*it)->getUID()](const auto &compUID) {
              return compUID == u;
            }
        );
      }
      if ((*it)->kind() == Component::Kind::Mesh) {
        gen::remove_if(
            mMeshes,
            [c = static_cast<Mesh *>(it->get())](const auto &d) { // NOLINT
              return d == c;
            }
        );
      }
      mComponentCache.get<IndexByUID>().erase((*it)->getUID());
      it = mComponents.erase(it);
    } else {
      (*it)->onEntityUpdate(*this, dt);
      ++it;
    }
  }

  addQueuedComponents();

  callDeferred();

  if (mComponents.empty()) mDestroyed = true;
}

void engine::Entity::drawSelf(sf::RenderTarget &target, sf::RenderStates states) const {
  for (const auto &d : mMeshes) {
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

const std::string &engine::Entity::getChannel() const noexcept {
  return mChannel;
}

void engine::Entity::addQueuedComponents() {
  while (!mQueuedComponents.empty()) {
    auto comp{std::move(mQueuedComponents.front())};
    mQueuedComponents.pop();

    comp->onStart(*this);
#ifdef DEBUG
    auto &idx{mComponentCache.get<IndexByName>()};
      if (idx.find(comp->getName()) != std::end(idx)) {
        System::logger().log("duplicate component name: ", comp->getName());
      }
#endif
    mComponentCache.get<IndexByUID>().insert(comp.get());
    mComponents.push_back(std::move(comp));
  }
}

void engine::Entity::callDeferred() {
  if (!mDeferred.empty()) {
    for (const auto &d : mDeferred) d();
    mDeferred.clear();
  }
}
