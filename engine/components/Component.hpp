//
// Created by marcin on 9/24/19.
//

#ifndef TYPEWRITER_GAEM_COMPONENT_HPP
#define TYPEWRITER_GAEM_COMPONENT_HPP

#include <memory>
#include <SFML/System/Time.hpp>
#include "../event_bus/EventBus.hpp"
#include "../Identifiable.hpp"

namespace engine {
class Entity;

class Component : public Identifiable {
public:
    using Ptr = std::unique_ptr<Component>;

    enum class Kind {
        Mesh,
        Physics,
        Script
    };

    Component();

    ~Component();

    void destroy();

    virtual Kind kind() const = 0;

    virtual void onStart(Entity &entity) = 0;

    virtual void onEntityUpdate(Entity &entity, sf::Time dt) = 0;

    bool destroyed() const noexcept;

    const std::string &getName() const;

    void setName(const std::string &name);

protected:
    template<class E = std::string, class F>
    void listen(const std::string &channel, F &&cb) const {
      EventBus::instance().listen<E>(channel, gen::to_uintptr(this), std::forward<F>(cb));
    }

    template<class E = std::string, class F>
    void listen(F &&cb) const {
      EventBus::instance().listen<E>(gen::to_uintptr(this), std::forward<F>(cb));
    }

    template<class E = std::string>
    void notify(E &&event = "") const {
      EventBus::instance().notify(std::forward<E>(event));
    }

    template<class E = std::string>
    void notify_channel(const std::string &channel, E &&event = "") const {
      EventBus::instance().notify(channel, std::forward<E>(event));
    }

protected:
    Entity *entity() const;

private:
    friend class Entity;

    Entity *mEntity;
    bool mDestroyed;
    std::string mName;
};

using ComponentPtr = Component::Ptr;
}

#endif //TYPEWRITER_GAEM_COMPONENT_HPP
