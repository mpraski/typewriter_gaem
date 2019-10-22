//
// Created by Marcin Praski on 22.10.19.
//

#ifndef TYPEWRITER_GAEM_NONMOVABLE_H
#define TYPEWRITER_GAEM_NONMOVABLE_H

namespace engine {
class NonMovable {
public:
    NonMovable(const NonMovable &) = delete;

    NonMovable &operator=(const NonMovable &) = delete;

protected:
    NonMovable() = default;

    ~NonMovable() = default;
};
}

#endif //TYPEWRITER_GAEM_NONMOVABLE_H
