//
// Created by marcin on 9/25/19.
//

#ifndef TYPEWRITER_GAEM_SCALE_HPP
#define TYPEWRITER_GAEM_SCALE_HPP

#include "../Transition.hpp"

namespace engine {
class Scale final : public Transition<sf::Vector2f> {
public:
    using Transition<sf::Vector2f>::Transition;

    bool dependent() const final {
      return true;
    }

protected:
    void onStart(Entity &entity) final {
      Transition::onStart(entity);
      mMesh = getTargetComponent<Mesh>();
    }

private:
    void applyStep(sf::Vector2f factor) final {
      mMesh->scale(factor);
    }

private:
    Mesh *mMesh;
};
}

#endif //TYPEWRITER_GAEM_SCALE_HPP
