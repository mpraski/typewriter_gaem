//
// Created by marcin on 8/19/19.
//

#ifndef TYPEWRITER_GAEM_SCENE_NODE_ANIMATION_HPP
#define TYPEWRITER_GAEM_SCENE_NODE_ANIMATION_HPP

#include "animation.hpp"

namespace engine {
template<class... Ts>
class scene_node_animation : public animation<Ts...>, public scene_node_animable {
    using builder = typename animation<Ts...>::builder;
public:
    explicit scene_node_animation(const builder &b)
        : animation<Ts...>(b),
          node{nullptr} {

    };

    ~scene_node_animation() override = default;

    void set_scene_node(scene_node *n) final {
      node = n;
    }

protected:
    scene_node *node;
};
}

#endif //TYPEWRITER_GAEM_SCENE_NODE_ANIMATION_HPP
