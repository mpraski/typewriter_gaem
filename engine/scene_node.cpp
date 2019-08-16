//
// Created by marcin on 8/15/19.
//

#include "scene_node.hpp"

engine::scene_node::scene_node()
    : bounds{},
      children{},
      parent{nullptr} {

}

engine::scene_node::scene_node(sf::FloatRect bounds)
    : bounds{bounds},
      children{},
      parent{nullptr} {

}

void engine::scene_node::attach_child(engine::scene_node::scene_node_ptr ptr) {
  ptr->parent = this;
  children.push_back(std::move(ptr));
}

engine::scene_node_ptr engine::scene_node::detach_child(const engine::scene_node &node) {
  auto child{gen::find(children, [&](const auto &ptr) { return ptr.get() == &node; })};
  if (child == std::end(children)) {
    throw std::runtime_error("Child scene node not present");
  }

  auto evicted{std::move(*child)};
  evicted->parent = nullptr;
  children.erase(child);
  return evicted;
}

sf::Transform engine::scene_node::global_transform() const {
  auto transform{sf::Transform::Identity};
  for (const auto *ptr{this}; ptr != nullptr; ptr = ptr->parent) {
    transform *= parent->getTransform();
  }
  return transform;
}

sf::FloatRect engine::scene_node::local_bounds() const {
  return bounds;
}

sf::FloatRect engine::scene_node::global_bounds() const {
  return global_transform().transformRect(local_bounds());
}

bool engine::scene_node::is_root() const {
  return !parent;
}

bool engine::scene_node::parent_intersects(const sf::FloatRect &rect) const {
  return parent ? parent->global_bounds().intersects(rect) : false;
}

bool engine::scene_node::parent_contains(const sf::Vector2f &vert) const {
  return parent ? parent->global_bounds().contains(vert) : false;
}

void engine::scene_node::update(sf::Time dt) {
  update_self(dt);
  update_children(dt);
}

void engine::scene_node::update_self(sf::Time dt) {

}

void engine::scene_node::draw_self(sf::RenderTarget &target, sf::RenderStates &states) const {

}

void engine::scene_node::update_children(sf::Time dt) {
  for (const auto &ptr : children) {
    ptr->update(dt);
  }
}

void engine::scene_node::draw_children(sf::RenderTarget &target, sf::RenderStates &states) const {
  for (const auto &ptr : children) {
    ptr->draw(target, states);
  }
}

void engine::scene_node::draw(sf::RenderTarget &target, sf::RenderStates states) const {
  states.transform *= getTransform();

  draw_self(target, states);
  draw_children(target, states);
}