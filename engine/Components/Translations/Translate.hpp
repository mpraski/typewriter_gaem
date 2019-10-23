//
// Created by marcin on 9/24/19.
//

#ifndef TYPEWRITER_GAEM_TRANSLATE_HPP
#define TYPEWRITER_GAEM_TRANSLATE_HPP

#include "../Transition.hpp"

namespace engine {
class Translate final : public Transition<float, float> {
public:
    using Transition<float, float>::Transition;

    bool dependent() const final {
      return true;
    }

protected:
    void onStart(Entity &entity) final {
      Transition::onStart(entity);
      mMesh = getTargetComponent<Mesh>();
    }

private:
    void applyStep(float x, float y) final {
      mMesh->move(x, y);
    }

private:
    Mesh *mMesh;
};
}

#endif //TYPEWRITER_GAEM_TRANSLATE_HPP
