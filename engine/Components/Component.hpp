//
// Created by marcin on 9/24/19.
//

#ifndef TYPEWRITER_GAEM_COMPONENT_HPP
#define TYPEWRITER_GAEM_COMPONENT_HPP

#include <memory>
#include <cassert>
#include <SFML/System/Time.hpp>
#include "../System.hpp"
#include "../Identifiable.hpp"

namespace engine {
class Entity;

class Component : public Identifiable {
public:
    using Ptr = std::unique_ptr<Component>;

    enum class Kind {
        Mesh,
        Physics,
        Script,
        Interactive
    };

public:
    Component();

    explicit Component(std::string name);

    void markDestroyed();

    bool destroyed() const noexcept;

    void addDependentComponent(sf::Uint64 id);

    const std::vector<sf::Uint64> &getDependentComponents() const noexcept;

    const std::string &getChannel() const noexcept;

    virtual bool dependent() const;

    virtual Kind kind() const = 0;

    virtual void onStart(Entity &entity) = 0;

    virtual void onEntityUpdate(Entity &entity, sf::Time dt) = 0;

    virtual ~Component() = default;

protected:
    template<typename T = Component>
    T *getTargetComponent() const {
      T *target = dynamic_cast<T *>(mTargetComponent);
      if (!target) {
        throw std::runtime_error(gen::str("Target component is not a ", typeid(T).name()));
      }
      return target;
    }

    template<typename E = std::true_type, typename F>
    void listen(const std::string &channel, F &&cb) const {
      System::bus().listen<E>(channel, gen::to_uintptr(this), std::forward<F>(cb));
    }

    template<typename E>
    void notify(E &&event) const {
      System::bus().notify(std::forward<E>(event));
    }

    void notifyChannel(const std::string &channel) const;

    template<typename E>
    void notifyChannel(const std::string &channel, E &&event) const {
      System::bus().notify(channel, std::forward<E>(event));
    }

private:
    friend class Entity;

    bool mDestroyed;
    Component *mTargetComponent;
    std::string mChannel;
    std::vector<sf::Uint64> mDependentComponents;
};

using ComponentPtr = Component::Ptr;
}

#endif //TYPEWRITER_GAEM_COMPONENT_HPP
