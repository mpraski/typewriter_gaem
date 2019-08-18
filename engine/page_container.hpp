//
// Created by marcin on 8/9/19.
//

#ifndef TYPEWRITER_GAEM_PAGE_CONTAINER_HPP
#define TYPEWRITER_GAEM_PAGE_CONTAINER_HPP

#include <iostream>
#include "game_object.hpp"
#include "scene_node.hpp"
#include "story/story.hpp"
#include "page.hpp"

namespace engine {
class page_container : public game_object, public scene_node {
public:
    page_container(
        system_ptr sys_ptr,
        audio_system_ptr audio_ptr,
        story_ptr story_ptr
    );

private:
    mutable sf::VertexArray debug_bounds_vertices;

    void draw_self(sf::RenderTarget &target, sf::RenderStates states) const final;

    void draw_page_outline() const;
};

template<class ...Ts>
static auto make_page_container(Ts &&... args) {
  return std::make_unique<page_container>(std::forward<Ts>(args)...);
}
}

#endif //TYPEWRITER_GAEM_PAGE_CONTAINER_HPP
