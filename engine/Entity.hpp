//
// Created by marcin on 8/15/19.
//

#ifndef TYPEWRITER_GAEM_ENTITY_HPP
#define TYPEWRITER_GAEM_ENTITY_HPP

#include <SFML/Graphics.hpp>
#include "System.hpp"
#include "utilities/General.hpp"
#include "event_bus/EventBus.hpp"
#include "components/Component.hpp"
#include "components/Interactive.hpp"

namespace engine {
class Entity final : public Identifiable,
                     public sf::Drawable,
                     public sf::Transformable,
                     public sf::NonCopyable {
public:
    using Ptr = std::unique_ptr<Entity>;

    Entity();

    ~Entity();

    void update(sf::Time dt);

    void destroy();

    void addChild(Ptr ptr);

    Ptr removeChild(const Entity &entity);

    template<typename T, typename U = Component>
    void addComponent(std::unique_ptr<T> component, const U *targetComponent = nullptr) {
      static_assert(std::is_convertible_v<Component *, T *>, "T must derive from Component");
      static_assert(std::is_convertible_v<Component *, U *>, "U must derive from Component");
      ComponentPtr c{static_cast<Component *>(component.release())};
      if (c->kind() == Component::Kind::Mesh) {
        mDrawables.push_back(dynamic_cast<sf::Drawable *>(c.get()));
      }
      switch (c->kind()) {
        case Component::Kind::Mesh:
          mDrawables.push_back(dynamic_cast<sf::Drawable *>(c.get()));
          break;
        case Component::Kind::Interactive:
          mInteractives.push_back(dynamic_cast<Interactive *>(c.get()));
          break;
        default:
          break;
      }
      mComponentByUID[c->getUID()] = c.get();
      c->mEntity = this;
      c->mTargetComponent = targetComponent;
      c->onStart(*this);
      if (targetComponent) {
        targetComponent->addDependentComponent(c->getUID());
      }
      mComponents.push_back(std::move(c));
    }

    template<typename T = Component>
    T *getComponent(const std::string &name) {
      static_assert(std::is_convertible_v<Component *, T *>, "T must derive from Component");
      if (name.empty()) return nullptr;
      auto comp{gen::find_if(mComponents, [&](const auto &c) { c->getName() == name; })};
      if (comp == std::end(mComponents)) return nullptr;
      return dynamic_cast<T *>(comp->get());
    }

    template<typename T = Component>
    T *getComponent(sf::Uint64 id) {
      static_assert(std::is_convertible_v<Component *, T *>, "T must derive from Component");
      if (!id) return nullptr;
      auto comp{mComponentByUID.find(id)};
      if (comp == std::end(mComponentByUID)) return nullptr;
      return dynamic_cast<T *>(comp);
    }

    template<typename T = Component, typename U = Component>
    std::unique_ptr<T> removeComponent(const U &component) {
      static_assert(std::is_convertible_v<Component *, T *>, "T must derive from Component");
      static_assert(std::is_convertible_v<Component *, U *>, "U must derive from Component");
      auto *cast_component{static_cast<Component *>(component)};
      auto comp{gen::find_if(mComponents, [&](const auto &c) { return c.get() == cast_component; })};
      if (comp == std::end(mComponents)) {
        throw std::runtime_error("Component not present");
      }
      if (!comp->getDependentComponents().empty()) {
        throw std::runtime_error("Component has dependants");
      }
      switch (c->kind()) {
        case Component::Kind::Mesh:
          gen::remove_if(mDrawables, [&](const auto &d) {
            return static_cast<Component *>(d) == cast_component;
          });
          break;
        case Component::Kind::Interactive:
          gen::remove_if(mInteractives, [&](const auto &d) {
            return static_cast<Component *>(d) == cast_component;
          });
          break;
        default:
          break;
      }
      mComponentByUID.erase(comp->getUID());
      if (comp->mTargetComponent) {
        gen::remove_if(comp->mTargetComponent->mDependentComponents, [&](const auto &compUID) {
          compUID == comp->getUID();
        });
      }
      auto evicted{std::move(*comp)};
      mComponents.erase(comp);
      return std::unique_ptr<T>{static_cast<T *>(evicted.release())};
    }

    sf::Transform globalTransform() const;

    sf::Vector2f globalPosition() const;

    bool hasElapsed(sf::Time dt);

    bool root() const noexcept;

    bool destroyed() const noexcept;

    const std::string &getEntityChannel() const;

private:
    void draw(sf::RenderTarget &target, sf::RenderStates states) const final;

    void updateChildren(sf::Time dt);

    void drawChildren(sf::RenderTarget &target, sf::RenderStates states) const;

    virtual void updateSelf(sf::Time dt);

    virtual void drawSelf(sf::RenderTarget &target, sf::RenderStates states) const;

    void performMouseHover();

    void performMouseClick();

private:
    const Entity *mParent;
    bool mDestroyed;

    std::string mEntityChannel;

    std::vector<Ptr> mChildren;
    std::vector<ComponentPtr> mComponents;
    std::vector<sf::Drawable *> mDrawables;
    std::vector<Interactive *> mInteractives;

    std::unordered_map<sf::Uint64, Component *> mComponentByUID;

    mutable sf::Clock mClock;
    mutable sf::Time mSinceLastUpdate;
};

using EntityPtr = Entity::Ptr;
}

#endif //TYPEWRITER_GAEM_ENTITY_HPP
