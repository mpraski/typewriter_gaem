//
// Created by marcin on 7/23/19.
//

#ifndef TYPEWRITER_GAEM_AUDIOSYSTEM_HPP
#define TYPEWRITER_GAEM_AUDIOSYSTEM_HPP

#include <random>
#include <chrono>
#include "Engine.hpp"

namespace engine {
class AudioSystem final {
public:
    static const auto &instance() {
      static AudioSystem audioSystem{};
      return audioSystem;
    }

    AudioSystem(const AudioSystem &p) = delete;

    AudioSystem(AudioSystem &&p) = delete;

    AudioSystem &operator=(const AudioSystem &p) = delete;

    AudioSystem &operator=(AudioSystem &&p) = delete;

    void playTypewriterClick() const;

    void playTypewriterSpace() const;

private:
    AudioSystem();

    void playCached(const std::string &key, const sf::SoundBuffer &buf) const;

    template<class It>
    It random(It begin, It end) const {
      long sz{std::distance(begin, end)};
      long div{RAND_MAX / sz};

      long k;
      do { k = mRandFun() / div; } while (k >= sz);

      return std::next(begin, k);
    }

private:
    const System::ResourcePack<sf::SoundBuffer> &mTypewriterClicks;
    mutable System::ResourcePack<sf::Sound> mSoundCache;
    std::default_random_engine mRandEngine;
    std::uniform_int_distribution<unsigned> mRandDist;
    std::function<unsigned()> mRandFun;
};
}

#endif //TYPEWRITER_GAEM_AUDIOSYSTEM_HPP
