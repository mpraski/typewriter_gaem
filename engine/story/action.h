//
// Created by marcin on 7/27/19.
//

#ifndef TYPEWRITER_GAEM_ACTION_H
#define TYPEWRITER_GAEM_ACTION_H

#include <boost/uuid/uuid.hpp>

namespace engine {
class action {
public:
    enum class kind {
      DIALOG
    };

    kind kind;
    boost::uuids::uuid pid;
};
}

#endif //TYPEWRITER_GAEM_ACTION_H
