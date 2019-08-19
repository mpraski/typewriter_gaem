//
// Created by marcin on 8/8/19.
//

#ifndef TYPEWRITER_GAEM_TRANSLATE_HPP
#define TYPEWRITER_GAEM_TRANSLATE_HPP

#include <SFML/Graphics.hpp>
#include "scene_node_animation.hpp"
#include "../scene_node.hpp"

namespace engine {
class translate : public scene_node_animation<float, float> {
public:
    using scene_node_animation<float, float>::scene_node_animation;

protected:
    void apply_step(float x, float y) const final {
      node->move(x, y);
    }
};

class translate_vertical : public scene_node_animation<float> {
public:
    using scene_node_animation<float>::scene_node_animation;

protected:
    void apply_step(float y) const final {
      node->move(0.f, y);
    }
};

class translate_horizontal : public scene_node_animation<float> {
public:
    using scene_node_animation<float>::scene_node_animation;

protected:
    void apply_step(float x) const final {
      node->move(x, 0.f);
    }
};
}

#endif //TYPEWRITER_GAEM_TRANSLATE_HPP
