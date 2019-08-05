//
// Created by marcin on 8/1/19.
//

#ifndef TYPEWRITER_GAEM_CENTER_H
#define TYPEWRITER_GAEM_CENTER_H

#include "printable.h"
#include "../text_effect.h"
#include "../utilities/general.h"

namespace engine {
class center : public printable {
public:
    center(
        const resources_ptr &rptr,
        const std::wstring &c
    );

    printable *clone() const override;

private:
    float word_width(const std::wstring &word) const;
};
}

#endif //TYPEWRITER_GAEM_CENTER_H
