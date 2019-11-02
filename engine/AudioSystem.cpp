//
// Created by marcin on 7/23/19.
//

#include "AudioSystem.hpp"

engine::AudioSystem::AudioSystem()
    : mSoundCache{},
      mRandEngine{static_cast<unsigned>(std::chrono::system_clock::now().time_since_epoch().count())},
      mRandDist{0, RAND_MAX},
      mRandFun{[&] {
        return mRandDist(mRandEngine);
      }},
      mHash{} {
}

void engine::AudioSystem::playRandom(const std::string &soundCategory) const {
  const auto &soundPack{System::instance().get_sounds_category(soundCategory)};

  auto rand_buf{gen::random_iter(std::begin(soundPack),
                                 std::end(soundPack),
                                 mRandFun)};

  auto seed{mHash(rand_buf->first)};
  if (auto it{mSoundCache.find(seed)}; it != mSoundCache.end()) {
    it->second.play();
  } else {
    if (auto[itt, ok]{mSoundCache.emplace(seed, rand_buf->second)}; ok) {
      itt->second.play();
    }
  }
}

void engine::AudioSystem::play(const std::string &soundCategory, const std::string &soundName) const {
  size_t seed{};
  boost::hash_combine(seed, soundCategory);
  boost::hash_combine(seed, soundName);

  if (auto it{mSoundCache.find(seed)}; it != mSoundCache.end()) {
    it->second.play();
  } else {
    const auto &buf{System::instance().get_sounds(soundName, soundCategory)};
    if (auto[itt, ok]{mSoundCache.emplace(seed, buf)}; ok) {
      itt->second.play();
    }
  }
}

void engine::AudioSystem::play(const std::string &soundName) const {
  auto seed{mHash(soundName)};
  if (auto it{mSoundCache.find(seed)}; it != mSoundCache.end()) {
    it->second.play();
  } else {
    const auto &buf{System::instance().get_sounds(soundName)};
    if (auto[itt, ok]{mSoundCache.emplace(seed, buf)}; ok) {
      itt->second.play();
    }
  }
}
