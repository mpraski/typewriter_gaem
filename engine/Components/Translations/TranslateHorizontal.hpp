//
// Created by marcin on 9/24/19.
//

#ifndef TYPEWRITER_GAEM_TRANSLATEHORIZONTAL_HPP
#define TYPEWRITER_GAEM_TRANSLATEHORIZONTAL_HPP

#include "../Transition.hpp"

namespace engine {
class TranslateHorizontal final : public Transition<float> {
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
    void applyStep(float x) final {
      mMesh->move(x, 0.f);
    }

private:
    Mesh *mMesh;
};
}

#endif //TYPEWRITER_GAEM_TRANSLATEHORIZONTAL_HPP
