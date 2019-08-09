//
// Created by marcin on 7/23/19.
//

#ifndef TYPEWRITER_GAEM_GAME_STATE_HPP
#define TYPEWRITER_GAEM_GAME_STATE_HPP

#include "system.hpp"

namespace engine {
class game_state {
public:
    explicit game_state(const system_ptr &rptr);

protected:
    system_ptr system;
};
}

#endif //TYPEWRITER_GAEM_GAME_STATE_HPP
