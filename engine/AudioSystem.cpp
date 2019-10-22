//
// Created by marcin on 7/23/19.
//

#include "AudioSystem.hpp"

engine::AudioSystem::AudioSystem()
    : mTypewriterClicks{System::instance().get_sounds_category("typewriter_clicks")},
      mSoundCache{},
      mRandEngine{static_cast<unsigned>(std::chrono::system_clock::now().time_since_epoch().count())},
      mRandDist{0, RAND_MAX},
      mRandFun{[&] {
        return mRandDist(mRandEngine);
      }} {
  if (mTypewriterClicks.empty()) {
    throw std::runtime_error("No typewriter click AudioSystem found");
  }
}

void engine::AudioSystem::playTypewriterClick() const {
  auto rand_buf{gen::random_iter(std::begin(mTypewriterClicks),
                                 std::end(mTypewriterClicks),
                                 mRandFun)};

  playCached(rand_buf->first, rand_buf->second);
}

void engine::AudioSystem::playTypewriterSpace() const {
  constexpr auto key{"typewriter_spacebar"};
  const auto &buf{System::instance().get_sounds(key)};

  playCached(key, buf);
}

void engine::AudioSystem::playCached(const std::string &key, const sf::SoundBuffer &buf) const {
  if (auto it{mSoundCache.find(key)}; it != mSoundCache.end()) {
    it->second.play();
  } else if (auto[itt, ok]{mSoundCache.emplace(key, buf)}; ok) {
    itt->second.play();
  }
}