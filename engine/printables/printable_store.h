//
// Created by marcin on 8/1/19.
//

#ifndef TYPEWRITER_GAEM_PRINTABLE_STORE_H
#define TYPEWRITER_GAEM_PRINTABLE_STORE_H

#include <functional>
#include "printable.h"

namespace engine {
class printable_store {
public:
    const std::function<void(printable_ptr &&)> &add;
    const std::function<void(size_t)> &remove;
};

}

#endif //TYPEWRITER_GAEM_PRINTABLE_STORE_H
