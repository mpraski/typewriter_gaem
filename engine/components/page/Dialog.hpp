//
// Created by marcin on 9/27/19.
//

#ifndef TYPEWRITER_GAEM_DIALOG_HPP
#define TYPEWRITER_GAEM_DIALOG_HPP

#include "Printable.hpp"

namespace engine {
class Dialog final : public Printable {
    Dialog(
        const std::wstring &person,
        const std::wstring &speech
    );

    bool interactive() final;

    void onHoverStart() final;

    void onHoverEnd() final;

    Action onClick() final;

    Action onPress() final;

private:
    bool mOn;
    bool mShouldUpdate;
    bool mInteractive;
    std::unordered_map<size_t, std::vector<TextEffect>> mEffectsOnHover;
    std::unordered_map<size_t, std::vector<TextEffect>> mEffectsOffHover;
};
}

#endif //TYPEWRITER_GAEM_DIALOG_HPP
