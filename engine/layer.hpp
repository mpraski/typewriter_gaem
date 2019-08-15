//
// Created by marcin on 8/15/19.
//

#ifndef TYPEWRITER_GAEM_LAYER_HPP
#define TYPEWRITER_GAEM_LAYER_HPP

#include <SFML/Graphics.hpp>

namespace engine {
class layer : public sf::Drawable, public sf::Transformable {
    using drawable_ptr = const sf::Drawable *;
    using layer_ptr = const layer *;
public:
    explicit layer(const sf::FloatRect &bounds);

    explicit layer(layer_ptr parent);

    layer(layer_ptr parent, const sf::FloatRect &bounds);

    layer(const layer &p) = delete;

    layer(layer &&p) = delete;

    layer &operator=(const layer &p) = delete;

    layer &operator=(layer &&p) = delete;

    sf::FloatRect local_bounds() const;

    sf::FloatRect global_bounds() const;

    bool is_root() const;

    bool parent_intersects(const sf::FloatRect &rect) const;

    bool parent_contains(const sf::Vector2f &vert) const;

    sf::FloatRect to_global(const sf::FloatRect& f) const;

protected:
    mutable sf::FloatRect bounds;

    template<class... Ts>
    void add(const Ts *... ts) {
      (drawables.push_back(ts), ...);
    }

    template<class... Ts>
    void add_buffered(Ts &&... ts) {
      (buffers.push_back(std::forward<Ts>(ts)), ...);
    }

    virtual void render(sf::RenderTarget &target, sf::RenderStates &states) const;

    virtual void post_render() const;

private:
    layer_ptr parent;
    std::vector<drawable_ptr> drawables;
    std::vector<std::pair<drawable_ptr, drawable_ptr>> buffers;

    void draw(sf::RenderTarget &target, sf::RenderStates states) const final;
};
}

#endif //TYPEWRITER_GAEM_LAYER_HPP
