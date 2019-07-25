//
// Created by marcin on 7/24/19.
//

#ifndef TYPEWRITER_GAEM_DIALOG_H
#define TYPEWRITER_GAEM_DIALOG_H

#include "printable.h"
#include "../text_effect.h"

namespace engine {
class dialog : public printable {
public:
    dialog(
        const resources_ptr &rptr,
        const std::wstring &person,
        const std::wstring &speech
    );

private:
    effect_map effects_on_hover;
    effect_map effects_off_hover;

    void on_hover_start() override;

    void on_hover_end() override;

    void on_click() override;
};
}

#endif //TYPEWRITER_GAEM_DIALOG_H
