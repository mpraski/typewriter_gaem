//
// Created by marcin on 8/15/19.
//

#include "scene_node.hpp"

engine::scene_node::scene_node()
    : bounds{},
      parent{nullptr},
      children{},
      animations{},
      clock{},
      since_last_update{sf::Time::Zero} {

}

engine::scene_node::scene_node(sf::FloatRect bounds)
    : bounds{bounds},
      parent{nullptr},
      children{},
      animations{},
      clock{},
      since_last_update{sf::Time::Zero} {

}

engine::scene_node::~scene_node() {
  event_bus::get_instance().unlisten_all(gen::to_uintptr(this));
}

void engine::scene_node::attach_child(scene_node_ptr ptr) {
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

void engine::scene_node::detach_animation(const std::string &name) {
  auto child{animations.find(name)};
  if (child == std::end(animations)) {
    throw std::runtime_error("Animation not present");
  }

  animations.erase(child);
}

engine::animable &engine::scene_node::get_animation(const std::string &name) const {
  return *animations.at(name).get();
}

sf::Transform engine::scene_node::global_transform() const {
  auto transform{sf::Transform::Identity};
  for (const auto *ptr{this}; ptr != nullptr; ptr = ptr->parent) {
    transform *= ptr->getTransform();
  }
  return transform;
}

sf::FloatRect engine::scene_node::local_bounds() const {
  return bounds;
}

sf::FloatRect engine::scene_node::global_bounds() const {
  return global_transform().transformRect(local_bounds());
}

sf::FloatRect engine::scene_node::parent_local_bounds() const {
  if (is_root()) {
    throw std::runtime_error("Trying to get parent bounds as root node 1");
  }

  return parent->local_bounds();
}

sf::FloatRect engine::scene_node::parent_global_bounds() const {
  if (is_root()) {
    throw std::runtime_error("Trying to get parent bounds as root node 2");
  }

  return parent->global_bounds();
}

bool engine::scene_node::has_elapsed(sf::Time dt) {
  if (since_last_update > dt) {
    since_last_update = sf::Time::Zero;
    return true;
  }

  return false;
}

bool engine::scene_node::is_root() const {
  return !parent;
}

bool engine::scene_node::parent_intersects(const sf::FloatRect &rect) const {
  if (is_root()) {
    throw std::runtime_error("Trying to check parent intersection as root");
  }

  return parent->global_bounds().intersects(rect);
}

bool engine::scene_node::parent_contains(const sf::Vector2f &vert) const {
  if (is_root()) {
    throw std::runtime_error("Trying to check parent containment as root");
  }

  return parent->global_bounds().contains(vert);
}

void engine::scene_node::update(sf::Time dt) {
  event_bus::get_instance().deliver();

  update_self(dt);
  update_children(dt);
}

void engine::scene_node::update_self(sf::Time dt) {

}

void engine::scene_node::draw_self(sf::RenderTarget &target, sf::RenderStates states) const {

}

void engine::scene_node::update_children(sf::Time dt) {
  for (const auto &ptr : children) {
    ptr->update(dt);
  }
}

void engine::scene_node::draw_children(sf::RenderTarget &target, sf::RenderStates states) const {
  for (const auto &ptr : children) {
    ptr->draw(target, states);
  }
}

void engine::scene_node::draw(sf::RenderTarget &target, sf::RenderStates states) const {
  states.transform *= getTransform();

  draw_self(target, states);
  draw_children(target, states);

  sf::Time elapsed{clock.restart()};
  since_last_update += elapsed;
}