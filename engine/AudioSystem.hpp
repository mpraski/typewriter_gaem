//
// Created by marcin on 7/23/19.
//

#ifndef TYPEWRITER_GAEM_AUDIOSYSTEM_HPP
#define TYPEWRITER_GAEM_AUDIOSYSTEM_HPP

#include <random>
#include <chrono>
#include "System.hpp"
#include "Utilities/General.hpp"
#include "Utilities/NonCopyable.h"
#include "Utilities/NonMovable.h"

namespace engine {
class AudioSystem final : private NonCopyable, private NonMovable {
public:
    static const auto &instance() {
      static AudioSystem audioSystem{};
      return audioSystem;
    }

    void playTypewriterClick() const;

    void playTypewriterSpace() const;

private:
    AudioSystem();

    void playCached(const std::string &key, const sf::SoundBuffer &buf) const;

private:
    const System::ResourcePack<sf::SoundBuffer> &mTypewriterClicks;
    mutable System::ResourcePack<sf::Sound> mSoundCache;
    std::default_random_engine mRandEngine;
    std::uniform_int_distribution<unsigned> mRandDist;
    std::function<unsigned()> mRandFun;
};
}

#endif //TYPEWRITER_GAEM_AUDIOSYSTEM_HPP
