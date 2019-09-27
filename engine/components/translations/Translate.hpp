//
// Created by marcin on 9/24/19.
//

#ifndef TYPEWRITER_GAEM_TRANSLATE_HPP
#define TYPEWRITER_GAEM_TRANSLATE_HPP

#include "../Transition.hpp"
#include "../../Entity.hpp"

namespace engine {
class Translate final : public Transition<float, float> {
public:
    using Transition<float, float>::Transition;

private:
    void applyStep(float x, float y) final {
      entity()->move(x, y);
    }
};
}

#endif //TYPEWRITER_GAEM_TRANSLATE_HPP
