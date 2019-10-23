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

    void markDestroyed();

    bool destroyed() const noexcept;

    const std::string &getName() const noexcept;

    void setName(const std::string &name);

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
      if constexpr(std::is_same_v<T, Component>) {
        return mTargetComponent;
      }

      T *target = dynamic_cast<T *>(mTargetComponent);
      if (!target) {
        throw std::runtime_error(gen::str("Target component is not a ", typeid(T).name()));
      }
      return target;
    }

    template<class E = std::string, class F>
    void listen(const std::string &channel, F &&cb) const {
      System::bus().listen<E>(channel, gen::to_uintptr(this), std::forward<F>(cb));
    }

    template<class E>
    void notify(E &&event) const {
      System::bus().notify(std::forward<E>(event));
    }

    template<class E = std::string>
    void notifyChannel(const std::string &channel, const E &event = gen::default_object<E>()) const {
      System::bus().notify(channel, event);
    }

    template<class E>
    void notifyChannel(const std::string &channel, E &&event) const {
      System::bus().notify(channel, std::forward<E>(event));
    }

private:
    friend class Entity;

    bool mDestroyed;
    Component *mTargetComponent;
    std::string mName;
    std::string mChannel;
    std::vector<sf::Uint64> mDependentComponents;
};

using ComponentPtr = Component::Ptr;
}

#endif //TYPEWRITER_GAEM_COMPONENT_HPP
