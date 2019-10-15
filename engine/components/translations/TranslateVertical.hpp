//
// Created by marcin on 9/24/19.
//

#ifndef TYPEWRITER_GAEM_TRANSLATEVERTICAL_HPP
#define TYPEWRITER_GAEM_TRANSLATEVERTICAL_HPP

#include "../Transition.hpp"
#include "../../Entity.hpp"

namespace engine {
class TranslateVertical final : public Transition<float> {
public:
    using Transition<float>::Transition;

    TranslateVertical *clone() const final {
      return new TranslateVertical{*this};
    }

private:
    void applyStep(float y) final {
      entity()->move(0.f, y);
    }
};
}

#endif //TYPEWRITER_GAEM_TRANSLATEVERTICAL_HPP
