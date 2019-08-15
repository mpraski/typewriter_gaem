//
// Created by marcin on 8/15/19.
//

#ifndef TYPEWRITER_GAEM_LAYER_HPP
#define TYPEWRITER_GAEM_LAYER_HPP

#include <SFML/Graphics.hpp>

namespace engine {
class layer : public sf::Drawable, public sf::Transformable {
    using drawable_ptr = const sf::Drawable *;
public:
    explicit layer(const sf::FloatRect &bounds);

    explicit layer(const layer *parent);

    layer(const layer *parent, const sf::FloatRect &bounds);

    sf::FloatRect local_bounds() const;

    sf::FloatRect global_bounds() const;

    bool parent_intersects(const sf::FloatRect &rect) const;

protected:
    const layer *parent;
    mutable sf::FloatRect bounds;

    template<class... Ts>
    void add(Ts const *__restrict__ ... ts) {
      (drawables.push_back(ts), ...);
    }

    template<class... Ts>
    void add_buffered(Ts &&... ts) {
      (buffers.push_back(std::forward<Ts>(ts)), ...);
    }

    virtual void render(sf::RenderTarget &target, sf::RenderStates &states) const;

    virtual void post_render() const;

private:
    std::vector<drawable_ptr> drawables;
    std::vector<std::pair<drawable_ptr, drawable_ptr>> buffers;

    void draw(sf::RenderTarget &target, sf::RenderStates states) const override;
};
}

#endif //TYPEWRITER_GAEM_LAYER_HPP
