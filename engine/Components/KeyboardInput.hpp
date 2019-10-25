//
// Created by Marcin Praski on 25.10.19.
//

#ifndef TYPEWRITER_GAEM_KEYBOARDINPUT_HPP
#define TYPEWRITER_GAEM_KEYBOARDINPUT_HPP

#include "Component.hpp"

namespace engine {
class KeyboardInput : public Component {
public:
    KeyboardInput();

    Kind kind() const override;

    void onStart(Entity &entity) override;

    void onEntityUpdate(Entity &entity, sf::Time dt) override;

private:
    constexpr static void registerEvents(KeyboardInput &k);
};
}

#endif //TYPEWRITER_GAEM_KEYBOARDINPUT_HPP
