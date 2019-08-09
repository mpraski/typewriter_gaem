//
// Created by marcin on 7/23/19.
//

#ifndef TYPEWRITER_GAEM_AUDIO_SYSTEM_HPP
#define TYPEWRITER_GAEM_AUDIO_SYSTEM_HPP

#include "game_state.hpp"
#include <random>

namespace engine {
class audio_system : public game_state {
public:
    explicit audio_system(const system_ptr &rptr);

    void play_typewriter_click() const;

    void play_typewriter_space() const;

private:
    const resource_pack<sf::SoundBuffer> *typewriter_clicks;
    mutable resource_pack<sf::Sound> sound_cache;
    std::default_random_engine rand_engine;
    std::uniform_int_distribution<unsigned> rand_dist;
    std::function<unsigned()> rand_fun;

    void play_with_cache(const std::string &key, const sf::SoundBuffer &buf) const;

    template<class It>
    inline It random(It begin, It end) const {
      long int sz{std::distance(begin, end)};
      long int div{RAND_MAX / sz};

      long int k;
      do { k = rand_fun() / div; } while (k >= sz);

      return std::next(begin, k);
    }
};
}

#endif //TYPEWRITER_GAEM_AUDIO_SYSTEM_HPP
