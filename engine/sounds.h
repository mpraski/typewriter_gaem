//
// Created by marcin on 7/23/19.
//

#ifndef TYPEWRITER_GAEM_SOUNDS_H
#define TYPEWRITER_GAEM_SOUNDS_H

#include "game_state.h"

namespace engine {
class sounds : public game_state {
public:
    explicit sounds(const resources_ptr &rptr);

    void play_typewriter_click() const;

private:
    const resource_pack<sf::SoundBuffer> *typewriter_clicks;
    mutable resource_pack<sf::Sound> sound_cache;

    template<class It>
    static It random(It begin, It end) {
      long int sz{std::distance(begin, end)};
      long int div{RAND_MAX / sz};

      size_t k;
      do { k = std::rand() / div; } while (k >= sz);

      return std::next(begin, k);
    }
};
};

#endif //TYPEWRITER_GAEM_SOUNDS_H
