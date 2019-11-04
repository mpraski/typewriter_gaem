//
// Created by marcin on 9/25/19.
//

#include "AnimatedMesh.hpp"

void engine::AnimatedMesh::onStart(engine::Entity &entity) {

}

void engine::AnimatedMesh::onEntityUpdate(engine::Entity &entity, sf::Time dt) {

}

engine::AnimatedMesh::AnimatedMesh(const sf::Texture &) {

}

void engine::AnimatedMesh::setNormalMap(const sf::Texture &normalMap) {
  mNormalMap = &normalMap;
}

void engine::AnimatedMesh::setFrameSize(const sf::Vector2u &frameSize) {
  mFrameSize = frameSize;
}

const sf::Vector2u &engine::AnimatedMesh::getFrameSize() const {
  return mFrameSize;
}

void engine::AnimatedMesh::setFrameCount(sf::Uint8 frameCount) {
  mFrameCount = frameCount;
}

sf::Uint8 engine::AnimatedMesh::getFrameCount() const {
  return mFrameCount;
}

void engine::AnimatedMesh::setFrameRate(float frameRate) {
  assert(frameRate >= 0.f);
  mFrameRate = frameRate;
}

float engine::AnimatedMesh::getFrameRate() const {
  return mFrameRate;
}

void engine::AnimatedMesh::lopp(bool loop) {
  mLoop = loop;
}

bool engine::AnimatedMesh::looped() const {
  return mLoop;
}

void engine::AnimatedMesh::setAnimation(const std::string &name) {
  auto it{mAnimations.find(name)};
  if (it == std::end(mAnimations)) {
    throw std::runtime_error("No animation named: " + name);
  }
  mFirstFrame = it->second.mBeginFrame;
  mLastFrame = it->second.mEndFrame;
}

void engine::AnimatedMesh::play(sf::Uint8 start, sf::Uint8 end, sf::Uint8 offset) {

}

bool engine::AnimatedMesh::playing() const {
  return mPlaying;
}

void engine::AnimatedMesh::pause(bool pause) {
  mPlaying = !pause;
}
