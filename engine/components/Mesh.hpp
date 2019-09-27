//
// Created by marcin on 9/25/19.
//

#ifndef TYPEWRITER_GAEM_MESH_HPP
#define TYPEWRITER_GAEM_MESH_HPP

#include "Component.hpp"
#include "Collider.hpp"
#include "../Entity.hpp"

namespace engine {
class Mesh : public Component,
             public sf::Drawable,
             public sf::Transformable {
public:
    Mesh();

    sf::FloatRect localBounds() const;

    sf::FloatRect globalBounds() const;

    Kind kind() const final;

    static std::unique_ptr<Collider> fromMesh(const Mesh &mesh);

protected:
    sf::FloatRect mBounds;
    mutable sf::VertexArray mVertices;
    mutable sf::VertexBuffer mVerticesBuffer;
    mutable sf::VertexArray mDebugBoundsVertices;

    const sf::Shader *mShader;
    const sf::Texture *mTexture;
    sf::BlendMode mBlendMode;

private:
    void draw(sf::RenderTarget &target, sf::RenderStates states) const final;
};
}

#endif //TYPEWRITER_GAEM_MESH_HPP
