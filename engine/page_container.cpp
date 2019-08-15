//
// Created by marcin on 8/9/19.
//

#include "page_container.hpp"

engine::page_container::page_container(const system_ptr &rptr, const story_ptr &sptr)
    : game_object{rptr},
      layer{{0, 0, rptr->page_width, rptr->page_height}},
      pg{rptr, sptr, this},
      debug_bounds_vertices{sf::Lines} {
  add(&pg, &debug_bounds_vertices);
}

void engine::page_container::run() {
  if (pg.can_advance()) {
    pg.advance();
  } else {
    pg.input();
  }
}

void engine::page_container::render(sf::RenderTarget &target, sf::RenderStates &states) const {
  debug_bounds_vertices.clear();
  draw_page_outline();
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