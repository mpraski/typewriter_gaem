//
// Created by marcin on 9/25/19.
//

#ifndef TYPEWRITER_GAEM_SCALE_HPP
#define TYPEWRITER_GAEM_SCALE_HPP

#include "../Transition.hpp"
#include "../../Entity.hpp"

namespace engine {
class Scale final : public Transition<sf::Vector2f> {
public:
    using Transition<sf::Vector2f>::Transition;

    Scale *clone() const final {
      return new Scale{*this};
    }

private:
    void applyStep(sf::Vector2f factor) final {
      entity()->scale(factor);
    }
};
}

#endif //TYPEWRITER_GAEM_SCALE_HPP
