//
// Created by marcin on 7/23/19.
//

#ifndef TYPEWRITER_GAEM_AUDIO_SYSTEM_H
#define TYPEWRITER_GAEM_AUDIO_SYSTEM_H

#include "game_state.h"

namespace engine {
class audio_system : public game_state {
public:
    explicit audio_system(const resources_ptr &rptr);

    void play_typewriter_click() const;

    void play_typewriter_space() const;

private:
    const resource_pack<sf::SoundBuffer> *typewriter_clicks;
    mutable resource_pack<sf::Sound> sound_cache;

    void play_with_cache(const std::string &key, const sf::SoundBuffer &buf) const;

    template<class It>
    static It random(It begin, It end) {
      long int sz{std::distance(begin, end)};
      long int div{RAND_MAX / sz};

      long int k;
      do { k = std::rand() / div; } while (k >= sz);

      return std::next(begin, k);
    }
};
}

#endif //TYPEWRITER_GAEM_AUDIO_SYSTEM_H
