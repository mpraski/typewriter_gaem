//
// Created by marcin on 9/25/19.
//

#ifndef TYPEWRITER_GAEM_ANIMATEDMESH_HPP
#define TYPEWRITER_GAEM_ANIMATEDMESH_HPP

#include <SFML/Config.hpp>
#include <string>
#include "Mesh.hpp"

namespace engine {
class AnimatedMesh : public Mesh {
public:
    class Animation final {

    private:
        std::string mName;
        sf::Uint16 mBeginFrame;
        sf::Int16 mEndFrame;
        bool mLoop;
    };

    void onStart(Entity &entity) final;

    void onEntityUpdate(Entity &entity, sf::Time dt) final;

private:
    std::vector<Animation> mAnimations;
};
}

#endif //TYPEWRITER_GAEM_ANIMATEDMESH_HPP
