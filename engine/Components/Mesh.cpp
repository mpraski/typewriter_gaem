//
// Created by marcin on 9/25/19.
//

#include <iostream>
#include "Mesh.hpp"

engine::Mesh::Mesh()
    : Component{},
      mBounds{},
      mVertices{sf::Triangles},
      mDebugBoundsVertices{sf::Lines},
      mShader{},
      mTexture{},
      mBlendMode{sf::BlendAdd} {

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
  states.transform *= getTransform();

#ifdef DEBUG_BOUNDS
  mDebugBoundsVertices.clear();
  gen::addRect(mDebugBoundsVertices, localBounds());
#endif

  if (mShader) {
    states.shader = mShader;
  }
  if (mTexture) {
    states.texture = mTexture;
  }
  states.blendMode = mBlendMode;

  target.draw(mVertices, states);

#ifdef DEBUG_BOUNDS
  target.draw(mDebugBoundsVertices, states);
#endif
}