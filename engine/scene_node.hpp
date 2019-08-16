//
// Created by marcin on 8/15/19.
//

#ifndef TYPEWRITER_GAEM_SCENE_NODE_HPP
#define TYPEWRITER_GAEM_SCENE_NODE_HPP

#include <SFML/Graphics.hpp>
#include "utilities/general.hpp"

namespace engine {
class scene_node : public sf::Drawable, public sf::Transformable, private sf::NonCopyable {
public:
    using scene_node_ptr = std::unique_ptr<scene_node>;

    scene_node();

    explicit scene_node(sf::FloatRect bounds);

    void update(sf::Time dt);

    void attach_child(scene_node_ptr ptr);

    scene_node_ptr detach_child(const scene_node &node);

    sf::Transform global_transform() const;

    sf::FloatRect local_bounds() const;

    sf::FloatRect global_bounds() const;

    bool is_root() const;

    bool parent_intersects(const sf::FloatRect &rect) const;

    bool parent_contains(const sf::Vector2f &vert) const;

protected:
    mutable sf::FloatRect bounds;

    void update_children(sf::Time dt);

    void draw_children(sf::RenderTarget &target, sf::RenderStates &states) const;

    virtual void update_self(sf::Time dt);

    virtual void draw_self(sf::RenderTarget &target, sf::RenderStates &states) const;

private:
    const scene_node *parent;
    std::vector<scene_node_ptr> children;

    void draw(sf::RenderTarget &target, sf::RenderStates states) const final;
};

using scene_node_ptr = scene_node::scene_node_ptr;
}

#endif //TYPEWRITER_GAEM_SCENE_NODE_HPP
