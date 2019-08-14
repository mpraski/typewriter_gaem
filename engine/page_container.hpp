//
// Created by marcin on 8/9/19.
//

#ifndef TYPEWRITER_GAEM_PAGE_CONTAINER_HPP
#define TYPEWRITER_GAEM_PAGE_CONTAINER_HPP

#include "game_object.hpp"
#include "story/story.hpp"
#include "page.hpp"

namespace engine {
class page_container : public game_object, public sf::Drawable, public sf::Transformable {
public:
    page_container(const system_ptr &rptr, const story_ptr &sptr);

    void run();

    sf::FloatRect local_bounds() const;

    sf::FloatRect global_bounds() const;

private:
    page pg;
    sf::FloatRect bounds;
    mutable sf::VertexArray debug_bounds_vertices;

    void draw(sf::RenderTarget &target, sf::RenderStates states) const override;

    void draw_page_outline() const;
};
}

#endif //TYPEWRITER_GAEM_PAGE_CONTAINER_HPP
