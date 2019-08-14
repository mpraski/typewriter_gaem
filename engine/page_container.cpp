//
// Created by marcin on 8/9/19.
//

#include "page_container.hpp"

engine::page_container::page_container(const system_ptr &rptr, const story_ptr &sptr)
    : game_object{rptr},
      pg{rptr, sptr, [&](const auto &rect) {
        return global_bounds().intersects(rect);
      }},
      bounds{0, 0, rptr->page_width, rptr->page_height},
      debug_bounds_vertices{sf::Lines} {

}

void engine::page_container::run() {
  if (pg.can_advance()) {
    pg.advance();
  } else {
    pg.input();
  }
}

sf::FloatRect engine::page_container::local_bounds() const {
  return bounds;
}

sf::FloatRect engine::page_container::global_bounds() const {
  return getTransform().transformRect(local_bounds());
}

void engine::page_container::draw(sf::RenderTarget &target, sf::RenderStates states) const {
  debug_bounds_vertices.clear();
  draw_page_outline();

  states.transform *= getTransform();
  gen::draw(target, states, pg, debug_bounds_vertices);
}

void engine::page_container::draw_page_outline() const {
  auto grey{sf::Color{100, 100, 100}};
  auto root_trans{global_bounds()};
  debug_bounds_vertices.append(
      sf::Vertex(sf::Vector2f(root_trans.left, root_trans.top), grey,
                 sf::Vector2f(1, 1)));
  debug_bounds_vertices.append(sf::Vertex(
      sf::Vector2f(root_trans.left, root_trans.top + root_trans.height),
      grey, sf::Vector2f(1, 1)));
  debug_bounds_vertices.append(sf::Vertex(
      sf::Vector2f(root_trans.left, root_trans.top + root_trans.height),
      grey, sf::Vector2f(1, 1)));
  debug_bounds_vertices.append(sf::Vertex(
      sf::Vector2f(root_trans.left + root_trans.width,
                   root_trans.top + root_trans.height),
      grey, sf::Vector2f(1, 1)));
  debug_bounds_vertices.append(sf::Vertex(
      sf::Vector2f(root_trans.left + root_trans.width,
                   root_trans.top + root_trans.height),
      grey, sf::Vector2f(1, 1)));
  debug_bounds_vertices.append(sf::Vertex(
      sf::Vector2f(root_trans.left + root_trans.width, root_trans.top),
      grey, sf::Vector2f(1, 1)));
  debug_bounds_vertices.append(sf::Vertex(
      sf::Vector2f(root_trans.left + root_trans.width, root_trans.top),
      grey, sf::Vector2f(1, 1)));
  debug_bounds_vertices.append(
      sf::Vertex(sf::Vector2f(root_trans.left, root_trans.top), grey,
                 sf::Vector2f(1, 1)));
}