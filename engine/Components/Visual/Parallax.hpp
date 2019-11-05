//
// Created by Marcin Praski on 05.11.19.
//

#ifndef TYPEWRITER_GAEM_PARALLAX_HPP
#define TYPEWRITER_GAEM_PARALLAX_HPP

#include "../Component.hpp"
#include "../PlayerController.hpp"

namespace engine {
class Parallax : public Component {
public:
    struct Scroll final {
        float x, y;
    };

    Parallax(float distance, float maxDistance);

    Kind kind() const override;

    void onStart(Entity &entity) override;

    void onEntityUpdate(Entity &entity, sf::Time dt) override;

private:
    float mDistance;
    float mMaxDistance;
    float mModifier;
};
}

#endif //TYPEWRITER_GAEM_PARALLAX_HPP
