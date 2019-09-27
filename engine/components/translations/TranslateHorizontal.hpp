//
// Created by marcin on 9/24/19.
//

#ifndef TYPEWRITER_GAEM_TRANSLATEHORIZONTAL_HPP
#define TYPEWRITER_GAEM_TRANSLATEHORIZONTAL_HPP

#include "../Transition.hpp"
#include "../../Entity.hpp"

namespace engine {
class TranslateHorizontal final : public Transition<float> {
public:
    using Transition<float>::Transition;

private:
    void applyStep(float x) final {
      entity()->move(x, 0.f);
    }
};
}

#endif //TYPEWRITER_GAEM_TRANSLATEHORIZONTAL_HPP
