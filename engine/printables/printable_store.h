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
    printable_store();

    printable_store(
        std::function<void(printable_ptr &&)> &&p,
        std::function<void(printable_ptr &&)> &&a,
        std::function<void(size_t)> &&t
    );

    std::function<void(printable_ptr &&)> populate;
    std::function<void(printable_ptr &&)> add;
    std::function<void(size_t)> truncate;
};

}

#endif //TYPEWRITER_GAEM_PRINTABLE_STORE_H
