//
// Created by marcin on 8/15/19.
//

#ifndef TYPEWRITER_GAEM_SCENE_NODE_HPP
#define TYPEWRITER_GAEM_SCENE_NODE_HPP

#include <any>
#include <SFML/Graphics.hpp>
#include "utilities/general.hpp"
#include "animation/translate.hpp"

namespace engine {
class scene_node : public sf::Drawable, public sf::Transformable, private sf::NonCopyable {
public:
    using scene_node_ptr = std::unique_ptr<scene_node>;
    using animation_ptr = std::unique_ptr<animable>;

    scene_node();

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

protected:
    mutable sf::FloatRect bounds;

    sf::Transform global_transform() const;

    sf::FloatRect local_bounds() const;

    sf::FloatRect global_bounds() const;

    bool has_elapsed(sf::Time dt);

    bool is_root() const;

    bool parent_intersects(const sf::FloatRect &rect) const;

    bool parent_contains(const sf::Vector2f &vert) const;

    void update_children(sf::Time dt);

    void draw_children(sf::RenderTarget &target, sf::RenderStates states) const;

    virtual void update_self(sf::Time dt);

    virtual void draw_self(sf::RenderTarget &target, sf::RenderStates states) const;

private:
    void draw(sf::RenderTarget &target, sf::RenderStates states) const final;

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
