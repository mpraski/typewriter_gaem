//
// Created by marcin on 9/25/19.
//

#ifndef TYPEWRITER_GAEM_ROTATE_HPP
#define TYPEWRITER_GAEM_ROTATE_HPP

#include "../Transition.hpp"
#include "../../Entity.hpp"

namespace engine {
class Rotate final : public Transition<float> {
public:
    using Transition<float>::Transition;

    Rotate *clone() const final {
      return new Rotate{*this};
    }

private:
    void applyStep(float angle) final {
      entity()->rotate(angle);
    }
};
}

#endif //TYPEWRITER_GAEM_ROTATE_HPP
