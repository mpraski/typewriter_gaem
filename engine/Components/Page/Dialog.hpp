//
// Created by marcin on 9/27/19.
//

#ifndef TYPEWRITER_GAEM_DIALOG_HPP
#define TYPEWRITER_GAEM_DIALOG_HPP

#include "Printable.hpp"
#include "../MouseInput.hpp"

namespace engine {
class Dialog final : public Printable, public MouseInput::Helper {
public:
    Dialog(
        const std::wstring &person,
        const std::wstring &speech
    );

    void onStart(Entity &entity) final;

    void onHoverStart() final;

    void onHoverEnd() final;

    void onClick() final;

private:
    std::unordered_map<size_t, std::vector<TextEffect>> mEffectsOnHover;
    std::unordered_map<size_t, std::vector<TextEffect>> mEffectsOffHover;
};
}

#endif //TYPEWRITER_GAEM_DIALOG_HPP
