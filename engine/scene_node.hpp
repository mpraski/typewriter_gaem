//
// Created by marcin on 8/15/19.
//

#ifndef TYPEWRITER_GAEM_SCENE_NODE_HPP
#define TYPEWRITER_GAEM_SCENE_NODE_HPP

#include <SFML/Graphics.hpp>
#include "utilities/general.hpp"
#include "animation/translate.hpp"
#include "event_bus/event_bus.hpp"

namespace engine {
class scene_node : public sf::Drawable, public sf::Transformable, private sf::NonCopyable {
public:
    using scene_node_ptr = std::unique_ptr<scene_node>;
    using animation_ptr = std::unique_ptr<animable>;

    scene_node();

    ~scene_node();

    explicit scene_node(sf::FloatRect bounds);

    void update(sf::Time dt);

    void attach_child(scene_node_ptr ptr);

    scene_node_ptr detach_child(const scene_node &node);

    // Some ugly duck typing here
    template<class T, class... Ts>
    void attach_animation(const std::string &name, Ts &&... args) {
      static_assert(std::is_convertible_v<T *, animable *>);
      static_assert(std::is_convertible_v<T *, positional_animable *>);

      auto animation{std::make_unique<T>(std::forward<Ts>(args)...)};
      animation->set_transformable(this);
      animations.insert(std::make_pair(name, std::move(animation)));
    }

    void detach_animation(const std::string &name);

    animable &get_animation(const std::string &name) const;

    template<class T>
    T &cast_animation(const std::string &name) const {
      auto *anim{animations.at(name).get()};
      assert(dynamic_cast<T *>(anim));
      auto *typed_anim{static_cast<T *>(anim)};
      return *typed_anim;
    }

protected:
    mutable sf::FloatRect bounds;

    sf::Transform global_transform() const;

    sf::FloatRect local_bounds() const;

    sf::FloatRect global_bounds() const;

    sf::FloatRect parent_local_bounds() const;

    sf::FloatRect parent_global_bounds() const;

    bool has_elapsed(sf::Time dt);

    bool is_root() const;

    bool parent_intersects(const sf::FloatRect &rect) const;

    bool parent_contains(const sf::Vector2f &vert) const;

    void update_children(sf::Time dt);

    void draw_children(sf::RenderTarget &target, sf::RenderStates states) const;

    virtual void update_self(sf::Time dt);

    virtual void draw_self(sf::RenderTarget &target, sf::RenderStates states) const;

    template<class E, class F>
    void listen(const std::string &channel, F &&cb) {
      auto &bus{event_bus::get_instance()};
      bus.listen<E>(channel, gen::to_uintptr(this), std::forward<F>(cb));
    }

    template<class E, class F>
    void listen(F &&cb) {
      auto &bus{event_bus::get_instance()};
      bus.listen<E>(gen::to_uintptr(this), std::forward<F>(cb));
    }

    template<class E>
    void notify(E &&event) {
      auto &bus{event_bus::get_instance()};
      bus.notify(std::forward<E>(event));
    }

    template<class E>
    void notify(const std::string &channel, E &&event) {
      auto &bus{event_bus::get_instance()};
      bus.notify(channel, std::forward<E>(event));
    }

    template<class E>
    void unlisten() {
      auto &bus{event_bus::get_instance()};
      bus.unlisten<E>(gen::to_uintptr(this));
    }

    template<class E>
    void unlisten(const std::string &channel) {
      auto &bus{event_bus::get_instance()};
      bus.unlisten<E>(channel, gen::to_uintptr(this));
    }

private:
    void draw(sf::RenderTarget &target, sf::RenderStates states) const final;

    bool animation_in_progress();

private:
    const scene_node *parent;
    std::vector<scene_node_ptr> children;
    mutable std::unordered_map<std::string, animation_ptr> animations;
    mutable sf::Clock clock;
    mutable sf::Time since_last_update;
};

using scene_node_ptr = scene_node::scene_node_ptr;
}

#endif //TYPEWRITER_GAEM_SCENE_NODE_HPP
