//
// Created by marcin on 7/27/19.
//

#ifndef TYPEWRITER_GAEM_STORY_H
#define TYPEWRITER_GAEM_STORY_H

#include <stack>
#include "printables/printable.h"

namespace engine {
class story {
private:
    std::stack<printable_ptr> story_printables;
};
}

#endif //TYPEWRITER_GAEM_STORY_H
