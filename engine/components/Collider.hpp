//
// Created by marcin on 9/25/19.
//

#ifndef TYPEWRITER_GAEM_COLLIDER_HPP
#define TYPEWRITER_GAEM_COLLIDER_HPP

#include "Component.hpp"
#include "../Entity.hpp"


namespace engine {
class Collider final : public Component {
public:
    class Event final {

    };

    Collider();

    sf::FloatRect localBounds() const;

    sf::FloatRect globalBounds() const;

    Kind kind() const final;

    void onStart(Entity &entity) final;

    void onEntityUpdate(Entity &entity, sf::Time dt) final;

private:
    void handleEvent(const Event &event);

private:
    sf::FloatRect mBounds;
    sf::Vector2f mPosition;
    sf::Vector2f mVelocity;
    float mMass;
    float mInvertedMass;
    bool mEnabled;
    bool mGlobal;
};
}

#endif //TYPEWRITER_GAEM_COLLIDER_HPP
