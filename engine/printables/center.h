//
// Created by marcin on 8/1/19.
//

#ifndef TYPEWRITER_GAEM_CENTER_H
#define TYPEWRITER_GAEM_CENTER_H

#include "printable.h"
#include "../text_effect.h"

namespace engine {
class center : public printable {
public:
    center(
        const resources_ptr &rptr,
        const std::wstring &c
    );
};
}

#endif //TYPEWRITER_GAEM_CENTER_H
