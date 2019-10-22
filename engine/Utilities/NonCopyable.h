//
// Created by Marcin Praski on 22.10.19.
//

#ifndef TYPEWRITER_GAEM_NONCOPYABLE_H
#define TYPEWRITER_GAEM_NONCOPYABLE_H

namespace engine {
class NonCopyable {
public:
    NonCopyable(const NonCopyable &) = delete;

    NonCopyable &operator=(const NonCopyable &) = delete;

protected:
    NonCopyable() = default;

    ~NonCopyable() = default;
};
}

#endif //TYPEWRITER_GAEM_NONCOPYABLE_H
