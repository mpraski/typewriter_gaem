//
// Created by marcin on 8/15/19.
//

#include "layer.hpp"

engine::layer::layer(const sf::FloatRect &bounds)
    : parent{nullptr},
      bounds{bounds},
      drawables{},
      buffers{} {

}

engine::layer::layer(const layer *parent)
    : parent{parent},
      bounds{},
      drawables{},
      buffers{} {

}

engine::layer::layer(const layer *parent, const sf::FloatRect &bounds)
    : parent{parent},
      bounds{bounds},
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
  return getTransform().transformRect(local_bounds());
}

bool engine::layer::parent_intersects(const sf::FloatRect &rect) const {
  if (parent) {
    return parent->global_bounds().intersects(rect);
  }
  return false;
}

void engine::layer::draw(sf::RenderTarget &target, sf::RenderStates states) const {
  states.transform *= getTransform();

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