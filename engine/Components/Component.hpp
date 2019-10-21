//
// Created by marcin on 9/24/19.
//

#ifndef TYPEWRITER_GAEM_COMPONENT_HPP
#define TYPEWRITER_GAEM_COMPONENT_HPP

#include <memory>
#include <cassert>
#include <SFML/System/Time.hpp>
#include "../EventBus/EventBus.hpp"
#include "../Identifiable.hpp"

namespace engine {
class Entity;

class Component : public Identifiable {
public:
    using Ptr = std::unique_ptr<Component>;
    using Adder = std::function<void(Ptr, Component *)>;

    enum class Kind {
        Mesh,
        Physics,
        Script,
        Interactive
    };

public:
    Component();

    Component(const Component &) = delete;

    Component &operator=(const Component &) = delete;

    void markDestroyed();

    bool destroyed() const noexcept;

    const std::string &getName() const noexcept;

    void setName(const std::string &name);

    const std::string &getChannel() const noexcept;

    virtual bool dependent() const;

    virtual Kind kind() const = 0;

    virtual void onStart(Entity &entity) = 0;

    virtual void onEntityUpdate(Entity &entity, sf::Time dt) = 0;

    void addDependentComponent(sf::Uint64 id);

    virtual ~Component() = default;

protected:
    Component *targetComponent() const;

    template<typename T, typename U = Component>
    void addComponent(std::unique_ptr<T> comp, U *target = nullptr) {
      Ptr p{static_cast<Component *>(comp.release())};
      mAdder(p, target);
    }

    template<class E = std::string, class F>
    void listen(const std::string &channel, F &&cb) const {
      EventBus::instance().listen<E>(channel, gen::to_uintptr(this), std::forward<F>(cb));
    }

    template<class E = std::string, class F>
    void listen(F &&cb) const {
      EventBus::instance().listen<E>(gen::to_uintptr(this), std::forward<F>(cb));
    }

    template<class E>
    void notify(E &event = gen::default_object<E>()) const {
      EventBus::instance().notify(std::forward<E>(event));
    }

    template<class E>
    void notify(E &&event) const {
      EventBus::instance().notify(std::forward<E>(event));
    }

    template<class E = std::string>
    void notifyChannel(const std::string &channel, E &event = gen::default_object<E>()) const {
      EventBus::instance().notify(channel, std::forward<E>(event));
    }

    template<class E>
    void notifyChannel(const std::string &channel, E &&event) const {
      EventBus::instance().notify(channel, std::forward<E>(event));
    }

private:
    friend class Entity;

    Adder mAdder;
    Component *mTargetComponent;
    bool mDestroyed;
    std::string mName;
    std::string mChannel;
    std::vector<sf::Uint64> mDependentComponents;
};

using ComponentPtr = Component::Ptr;
}

#endif //TYPEWRITER_GAEM_COMPONENT_HPP
