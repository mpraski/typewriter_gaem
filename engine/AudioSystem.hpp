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

    void playRandom(const std::string &soundCategory) const;

    void play(const std::string &soundCategory, const std::string &soundName) const;

    void play(const std::string &soundName) const;

private:
    AudioSystem();

private:
    mutable std::unordered_map<size_t, sf::Sound> mSoundCache;
    std::default_random_engine mRandEngine;
    std::uniform_int_distribution<unsigned> mRandDist;
    std::function<unsigned()> mRandFun;
    boost::hash<std::string> mHash;
};
}

#endif //TYPEWRITER_GAEM_AUDIOSYSTEM_HPP
