//
// Created by marcin on 7/23/19.
//

#ifndef TYPEWRITER_GAEM_AUDIO_SYSTEM_HPP
#define TYPEWRITER_GAEM_AUDIO_SYSTEM_HPP

#include "game_object.hpp"
#include <random>
#include <chrono>

namespace engine {
class audio_system : public game_object {
public:
    explicit audio_system(const system_ptr &rptr);

    void play_typewriter_click() const;

    void play_typewriter_space() const;

private:
    const resource_pack<sf::SoundBuffer> &typewriter_clicks;
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

using audio_system_ptr = std::shared_ptr<const audio_system>;

template<class ...Ts>
static auto make_audio_system(Ts &&... args) {
  return std::make_shared<const audio_system>(std::forward<Ts>(args)...);
}
}

#endif //TYPEWRITER_GAEM_AUDIO_SYSTEM_HPP
