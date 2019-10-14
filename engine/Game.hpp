//
// Created by marcin on 8/16/19.
//

#ifndef TYPEWRITER_GAEM_GAME_HPP
#define TYPEWRITER_GAEM_GAME_HPP


#include <SFML/System/Time.hpp>
#include <memory>
#include "Entity.hpp"

namespace engine {
class Game {
public:
    explicit Game(std::unique_ptr<Entity> rootEntity);

    void run();

private:
    void processEvents();

    void update(sf::Time dt);

    void draw();

private:
    static const sf::Time mTimePerFrame;
    static const constexpr auto mLayerCount = 1u;

    sf::RenderWindow &window;
    sf::View mWorldView;
    std::unique_ptr<Entity> mSceneGraph;
    std::array<std::unique_ptr<Entity>, mLayerCount> mSceneLayers;

    bool mHoldsMouse;
};
}

#endif //TYPEWRITER_GAEM_GAME_HPP
