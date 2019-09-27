//
// Created by marcin on 9/27/19.
//

#ifndef TYPEWRITER_GAEM_INTERACTIVE_HPP
#define TYPEWRITER_GAEM_INTERACTIVE_HPP

#include "Action.hpp"

namespace engine {
class Interactive {
    virtual bool interactive() = 0;

    virtual void onHoverStart() = 0;

    virtual void onHoverEnd() = 0;

    virtual Action onClick() = 0;

    virtual Action onPress() = 0;
};
}

#endif //TYPEWRITER_GAEM_INTERACTIVE_HPP
