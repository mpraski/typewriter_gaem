//
// Created by marcin on 8/9/19.
//

#ifndef TYPEWRITER_GAEM_PAGE_CONTAINER_HPP
#define TYPEWRITER_GAEM_PAGE_CONTAINER_HPP

#include "game_object.hpp"
#include "layer.hpp"
#include "story/story.hpp"
#include "page.hpp"

namespace engine {
class page_container : public game_object, public layer {
public:
    page_container(
        const system_ptr &rptr,
        const audio_system_ptr &aptr,
        const story_ptr &sptr
    );

    void animate();

private:
    page pg;
    mutable sf::VertexArray debug_bounds_vertices;

    void render(sf::RenderTarget &target, sf::RenderStates &states) const final;

    void draw_page_outline() const;
};
}

#endif //TYPEWRITER_GAEM_PAGE_CONTAINER_HPP
