//
// Created by marcin on 8/15/19.
//

#include "layer.hpp"

engine::layer::layer(const sf::FloatRect &bounds)
    : bounds{bounds},
      parent{nullptr},
      drawables{},
      buffers{} {

}

engine::layer::layer(const layer *parent)
    : bounds{},
      parent{parent},
      drawables{},
      buffers{} {

}

engine::layer::layer(const layer *parent, const sf::FloatRect &bounds)
    : bounds{bounds},
      parent{parent},
      drawables{},
      buffers{} {

}

void engine::layer::render(sf::RenderTarget &target, sf::RenderStates &states) const {

}

void engine::layer::post_render() const {

}

sf::FloatRect engine::layer::local_bounds() const {
  return bounds;
}

sf::FloatRect engine::layer::global_bounds() const {
  return to_global(local_bounds());
}

bool engine::layer::is_root() const {
  return !parent;
}

bool engine::layer::parent_intersects(const sf::FloatRect &rect) const {
  return parent ? parent->global_bounds().intersects(rect) : false;
}

bool engine::layer::parent_contains(const sf::Vector2f &vert) const {
  return parent ? parent->global_bounds().contains(vert) : false;
}

sf::FloatRect engine::layer::to_global(const sf::FloatRect &f) const {
  return getTransform().transformRect(is_root() ? f : parent->to_global(f));
}

void engine::layer::draw(sf::RenderTarget &target, sf::RenderStates states) const {
  if (!is_root()) {
    states.transform *= parent->getTransform();
  }

  render(target, states);

  for (const auto &[b, nb]: buffers) {
    if (sf::VertexBuffer::isAvailable()) {
      target.draw(*b, states);
    } else {
      target.draw(*nb, states);
    }
  }

  for (const auto &d: drawables) {
    target.draw(*d, states);
  }

  post_render();
}