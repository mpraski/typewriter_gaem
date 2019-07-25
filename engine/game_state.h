//
// Created by marcin on 7/23/19.
//

#ifndef TYPEWRITER_GAEM_GAME_STATE_H
#define TYPEWRITER_GAEM_GAME_STATE_H

#include "resources.h"

namespace engine {
class game_state {
public:
    explicit game_state(const resources_ptr &rptr);

protected:
    resources_ptr resources;
};
}

#endif //TYPEWRITER_GAEM_GAME_STATE_H