//
// Created by marcin on 9/25/19.
//

#ifndef TYPEWRITER_GAEM_ROTATE_HPP
#define TYPEWRITER_GAEM_ROTATE_HPP

#include "../Transition.hpp"

namespace engine {
class Rotate final : public Transition<float> {
public:
    using Transition<float>::Transition;

    bool dependent() const final {
      return true;
    }

protected:
    void onStart(Entity &entity) final {
      Transition::onStart(entity);

      mMesh = dynamic_cast<Mesh *>(targetComponent());
      if (!mMesh) {
        throw std::runtime_error("Target component is not a mesh");
      }
    }

private:
    void applyStep(float angle) final {
      mMesh->rotate(angle);
    }

private:
    Mesh *mMesh;
};
}

#endif //TYPEWRITER_GAEM_ROTATE_HPP
