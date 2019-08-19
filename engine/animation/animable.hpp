//
// Created by marcin on 8/19/19.
//

#ifndef TYPEWRITER_GAEM_ANIMABLE_HPP
#define TYPEWRITER_GAEM_ANIMABLE_HPP

namespace engine {
class scene_node;

class animable {
public:
    virtual ~animable() = default;

    virtual bool running() const = 0;

    virtual void start() = 0;

    virtual void step() = 0;
};

class scene_node_animable {
public:
    virtual ~scene_node_animable() = default;

    virtual void set_scene_node(scene_node *n) = 0;
};
}

#endif //TYPEWRITER_GAEM_ANIMABLE_HPP