//
// Created by marcin on 8/15/19.
//

#include "Entity.hpp"

engine::Entity::Entity()
    : Identifiable{},
      mParent{nullptr},
      mDestroyed{},
      mAdder{std::bind(
          &Entity::addComponent < Component, Component > ,
          this,
          std::placeholders::_1,
          std::placeholders::_2
      )},
      mEntityChannel{gen::str("ent-", getUID())},
      mChildren{},
      mComponents{},
      mDrawables{},
      mComponentByUID{},
      mClock{},
      mSinceLastUpdate{sf::Time::Zero} {

}

engine::Entity::~Entity() {
  destroy();
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
    c->markDestroyed();
  }
  mDestroyed = true;
}

void engine::Entity::updateSelf(sf::Time dt) {
  for (auto it{std::begin(mComponents)}; it != std::end(mComponents);) {
    if ((*it)->destroyed()) {
      for (const auto &depUID : (*it)->mDependentComponents) {
        if (auto depComp{getComponent(depUID)}; depComp) {
          depComp->markDestroyed();
        }
      }
      if ((*it)->dependent()) {
        gen::remove_if((*it)->mTargetComponent->mDependentComponents, [&](const auto &compUID) {
          return compUID == (*it)->getUID();
        });
      }
      switch ((*it)->kind()) {
        case Component::Kind::Mesh:
          gen::remove_if(mDrawables, [&](const auto &d) {
            return d == dynamic_cast<sf::Drawable *>(it->get());
          });
          break;
        case Component::Kind::Interactive:
          gen::remove_if(mInteractives, [&](const auto &d) {
            return d == dynamic_cast<Interactive *>(it->get());
          });
          break;
        default:
          break;
      }

      mComponentByUID.erase((*it)->getUID());
      it = mComponents.erase(it);
      (*it)->onEntityUpdate(*this, dt);
    } else {
      (*it)->onEntityUpdate(*this, dt);
      ++it;
    }
  }

  if (mComponents.empty()) {
    destroy();
  }

  performMouseHover();
  performMouseClick();
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

const std::string &engine::Entity::getEntityChannel() const {
  return mEntityChannel;
}

void engine::Entity::performMouseHover() {
  bool hovering{};
  for (auto &interactive : mInteractives) {
    if (interactive->interactive()) {
      auto bounds{interactive->globalBounds()};
      auto mousePos{System::instance().mousePosition()};
      if (bounds.contains(mousePos)) {
        interactive->onHoverStart();
      } else {
        interactive->onHoverEnd();
      }
    }
  }

  System::instance().setCursor(hovering
                               ? System::Cursor::HAND
                               : System::Cursor::ARROW);
}

void engine::Entity::performMouseClick() {
  if (System::instance().mouseClickAvailable()) {
    const auto &mcp{System::instance().mouseClickPosition()};
    for (auto &interactive : mInteractives) {
      if (interactive->interactive()) {
        auto bounds{interactive->globalBounds()};
        if (bounds.contains(mcp)) {
          interactive->onClick();
          System::instance().setCursor(System::Cursor::ARROW);
        }
      }
    }
  }
}

