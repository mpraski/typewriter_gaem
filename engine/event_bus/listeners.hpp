//
// Created by marcin on 8/18/19.
//

#ifndef TYPEWRITER_GAEM_LISTENERS_HPP
#define TYPEWRITER_GAEM_LISTENERS_HPP

#include <cstdint>

namespace engine {
using callback_id_t = std::uintptr_t;

class listeners {
public:
    virtual ~listeners() = default;

    virtual void deliver() = 0;

    virtual void remove(callback_id_t cbid) = 0;
};
}

#endif //TYPEWRITER_GAEM_LISTENERS_HPP
