//
// Created by marcin on 8/15/19.
//

#ifndef TYPEWRITER_GAEM_ENTITY_HPP
#define TYPEWRITER_GAEM_ENTITY_HPP

#include <boost/multi_index_container.hpp>
#include <boost/multi_index/composite_key.hpp>
#include <boost/multi_index/identity.hpp>
#include <boost/multi_index/hashed_index.hpp>
#include <boost/multi_index/mem_fun.hpp>
#include <SFML/Graphics.hpp>
#include "System.hpp"
#include "Identifiable.hpp"
#include "Utilities/General.hpp"
#include "EventBus/EventBus.hpp"
#include "Components/Component.hpp"
#include "Components/Mesh.hpp"

namespace engine {
class Entity final : public Identifiable,
                     public sf::Drawable,
                     public sf::Transformable {
    struct IndexByChannel {
    };
    struct IndexByUID {
    };
public:
    using Ptr = std::unique_ptr<Entity>;
    using Deferred = std::function<void()>;

    Entity();

    ~Entity();

    void update(sf::Time dt);

    void addChild(Ptr ptr);

    Ptr removeChild(const Entity &entity);

    template<typename Fun>
    void defer(Fun &&fun) {
      mDeferred.push_back(std::forward<Fun>(fun));
    }

    template<typename T, typename U = Component>
    void addComponent(std::unique_ptr<T> component, U *targetComponent = nullptr) {
      if constexpr(std::is_convertible_v<T *, Mesh *>) {
        mMeshes.push_back(static_cast<Mesh *>(component.get()));
      }
      ComponentPtr c{static_cast<Component *>(component.release())};
      if (c->dependent()) {
        c->mTargetComponent = targetComponent;
        c->mTargetComponent->addDependentComponent(c->getUID());
      }
      mQueuedComponents.push(std::move(c));
    }

    template<typename T = Component>
    T *getComponent(const std::string &channel) {
      if (channel.empty()) return nullptr;
      auto &idx{mComponentCache.get<IndexByChannel>()};
      auto comp{idx.find(channel)};
      if (comp == std::end(idx)) return nullptr;
      return dynamic_cast<T *>(*comp);
    }

    template<typename T = Component>
    T *getComponent(sf::Uint64 id) {
      if (!id) return nullptr;
      auto &idx{mComponentCache.get<IndexByUID>()};
      auto comp{idx.find(id)};
      if (comp == std::end(idx)) return nullptr;
      return dynamic_cast<T *>(*comp);
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
      if (comp->kind() == Component::Kind::Mesh) {
        gen::remove_if(mMeshes, [&](const auto &d) {
          return static_cast<Component *>(d) == cast_component;
        });
      }
      mComponentCache.get<IndexByUID>().erase(comp->getUID());
      if (comp->dependent()) {
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

    bool destroyed() const noexcept;

    const std::string &getChannel() const noexcept;

private:
    void draw(sf::RenderTarget &target, sf::RenderStates states) const final;

    void updateChildren(sf::Time dt);

    void drawChildren(sf::RenderTarget &target, sf::RenderStates states) const;

    inline void addQueuedComponents();

    inline void callDeferred();

    virtual void updateSelf(sf::Time dt);

    virtual void drawSelf(sf::RenderTarget &target, sf::RenderStates states) const;

private:
    bool mDestroyed;
    std::string mChannel;
    const Entity *mParent;

    std::vector<Ptr> mChildren;
    std::vector<ComponentPtr> mComponents;
    std::queue<ComponentPtr> mQueuedComponents;
    std::vector<Deferred> mDeferred;
    std::vector<Mesh *> mMeshes;

    boost::multi_index_container<
        Component *,
        indexed_by<
            hashed_unique<
                tag<IndexByChannel>,
                const_mem_fun<Component, const std::string &, &Component::getChannel>
            >,
            hashed_unique<
                tag<IndexByUID>,
                const_mem_fun<Identifiable, sf::Uint64, &Identifiable::getUID>
            >
        >
    > mComponentCache;

    mutable sf::Clock mClock;
    mutable sf::Time mSinceLastUpdate;
};

using EntityPtr = Entity::Ptr;
}

#endif //TYPEWRITER_GAEM_ENTITY_HPP
