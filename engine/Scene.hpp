//
// Created by Marcin Praski on 23.10.19.
//

#ifndef TYPEWRITER_GAEM_SCENE_HPP
#define TYPEWRITER_GAEM_SCENE_HPP

#include <cstddef>
#include "Entity.hpp"

namespace engine {
class Scene : public Identifiable, public sf::Drawable {
    Scene(std::vector<EntityPtr>&& layers);

private:
    std::vector<EntityPtr> mLayers;
};
}

#endif //TYPEWRITER_GAEM_SCENE_HPP
