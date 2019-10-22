//
// Created by marcin on 9/24/19.
//

#ifndef TYPEWRITER_GAEM_TRANSLATEVERTICAL_HPP
#define TYPEWRITER_GAEM_TRANSLATEVERTICAL_HPP

#include "../Transition.hpp"

namespace engine {
class TranslateVertical final : public Transition<float> {
public:
    using Transition<float>::Transition;

    bool dependent() const final {
      return true;
    }

protected:
    void onStart(Entity &entity) final {
      Transition::onStart(entity);
      mMesh = getTargetComponent<Mesh>();
    }

private:
    void applyStep(float y) final {
      mMesh->move(0.f, y);
    }

private:
    Mesh *mMesh;
};
}

#endif //TYPEWRITER_GAEM_TRANSLATEVERTICAL_HPP
