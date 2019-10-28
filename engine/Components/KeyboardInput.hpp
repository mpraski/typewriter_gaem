//
// Created by Marcin Praski on 25.10.19.
//

#ifndef TYPEWRITER_GAEM_KEYBOARDINPUT_HPP
#define TYPEWRITER_GAEM_KEYBOARDINPUT_HPP

#include "Component.hpp"

namespace engine {
class KeyboardInput final : public Component {
public:
    class Helper {
    public:
        Helper();

    protected:
        virtual void onKey(sf::Keyboard::Key) = 0;

        void captureKeyboardEvents(bool c);

        std::function<void(sf::Keyboard::Key)> getKeyboardListener();

    private:
        bool mCapture;
    };

public:
    KeyboardInput();

    explicit KeyboardInput(std::string name);

    Kind kind() const override;

    void onStart(Entity &entity) override;

    void onEntityUpdate(Entity &entity, sf::Time dt) override;

private:
    constexpr static void registerEvents(KeyboardInput &k);
};
}

#endif //TYPEWRITER_GAEM_KEYBOARDINPUT_HPP
