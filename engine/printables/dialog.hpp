//
// Created by marcin on 7/24/19.
//

#ifndef TYPEWRITER_GAEM_DIALOG_HPP
#define TYPEWRITER_GAEM_DIALOG_HPP

#include "printable.hpp"
#include "../text_effect.hpp"

namespace engine {
class dialog : public printable {
public:
    dialog(
        const system_ptr &rptr,
        const std::wstring &person,
        const std::wstring &speech
    );

private:
    bool on;
    bool update;
    effect_map effects_on_hover;
    effect_map effects_off_hover;

    printable *clone() const override;

    void on_hover_start() override;

    void on_hover_end() override;

    action on_click() override;
};
}

#endif //TYPEWRITER_GAEM_DIALOG_HPP
