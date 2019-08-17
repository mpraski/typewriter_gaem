//
// Created by marcin on 8/9/19.
//

#include "page_container.hpp"

engine::page_container::page_container(
    system_ptr sys_ptr,
    audio_system_ptr audio_ptr,
    story_ptr story_ptr
)
    : game_object{sys_ptr},
      scene_node{{0, 0, system->page_width, system->page_height}},
      debug_bounds_vertices{sf::Lines} {
  attach_child(make_page(sys_ptr, audio_ptr, story_ptr));
}

void engine::page_container::draw_self(sf::RenderTarget &target, sf::RenderStates states) const {
  debug_bounds_vertices.clear();
  draw_page_outline();

  target.draw(debug_bounds_vertices);
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