//
// Created by marcin on 8/16/19.
//

#ifndef TYPEWRITER_GAEM_GAME_HPP
#define TYPEWRITER_GAEM_GAME_HPP


#include <SFML/System/Time.hpp>
#include "Entity.hpp"
#include "Entity.hpp"
#include "story/story.hpp"
#include "page_container.hpp"

namespace engine {
class game : GameObject {
public:
    game(
        const SystemPtr &rptr,
        const audio_system_ptr &aptr,
        const story_ptr &sptr
    );

    void run();

private:
    void process_events();

    void update(sf::Time dt);

    void draw();

private:
    static const sf::Time time_per_frame;
    static const constexpr auto layer_count = 1u;

    sf::RenderWindow &window;
    sf::View world_view;
    Entity scene_graph;
    std::array<Entity *, layer_count> scene_layers;

    bool holds_mouse;
};
}

#endif //TYPEWRITER_GAEM_GAME_HPP
