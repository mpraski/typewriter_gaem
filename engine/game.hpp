//
// Created by marcin on 8/16/19.
//

#ifndef TYPEWRITER_GAEM_GAME_HPP
#define TYPEWRITER_GAEM_GAME_HPP


#include <SFML/System/Time.hpp>
#include "scene_node.hpp"
#include "game_object.hpp"
#include "scene_node.hpp"
#include "story/story.hpp"
#include "page_container.hpp"

namespace engine {
class game : game_object {
public:
    game(
        const system_ptr &rptr,
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
    scene_node scene_graph;
    std::array<scene_node *, layer_count> scene_layers;

    bool holds_mouse;
};
}

#endif //TYPEWRITER_GAEM_GAME_HPP
