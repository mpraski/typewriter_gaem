//
// Created by marcin on 9/25/19.
//

#include "Mesh.hpp"

engine::Mesh::Mesh()
    : mBounds{},
      mVertices{sf::Triangles},
      mVerticesBuffer{sf::VertexBuffer::Dynamic},
      mDebugBoundsVertices{sf::Triangles},
      mShader{},
      mTexture{},
      mBlendMode{sf::BlendNone} {

}

sf::FloatRect engine::Mesh::localBounds() const {
  return mBounds;
}

sf::FloatRect engine::Mesh::globalBounds() const {
  return getTransform().transformRect(localBounds());
}

engine::Component::Kind engine::Mesh::kind() const {
  return Component::Kind::Mesh;
}

void engine::Mesh::draw(sf::RenderTarget &target, sf::RenderStates states) const {
#ifdef DEBUG_BOUNDS
  mDebugBoundsVertices.clear();
  gen::addRect(mDebugBoundsVertices, globalBounds());
#endif

  if (sf::VertexBuffer::isAvailable()) {
    if (mVerticesBuffer.getVertexCount() != mVertices.getVertexCount())
      mVerticesBuffer.create(mVertices.getVertexCount());

    if (mVertices.getVertexCount() > 0)
      mVerticesBuffer.update(&mVertices[0]);
  }

  if (mShader) {
    states.shader = mShader;
  }
  if (mTexture) {
    states.texture = mTexture;
  }
  states.blendMode = mBlendMode;

  if (sf::VertexBuffer::isAvailable()) {
    target.draw(mVerticesBuffer, states);
  } else {
    target.draw(mVertices, states);
  }

#ifdef DEBUG_BOUNDS
  target.draw(mDebugBoundsVertices, states);
#endif
}