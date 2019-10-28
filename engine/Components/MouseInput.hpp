//
// Created by Marcin Praski on 26.10.19.
//

#ifndef TYPEWRITER_GAEM_MOUSEINPUT_HPP
#define TYPEWRITER_GAEM_MOUSEINPUT_HPP

#include "Component.hpp"

namespace engine {
class MouseInput final : public Component {
public:
    enum class EventKind {
        Hover,
        Click
    };

    struct Event final {
        EventKind kind;
        sf::Vector2f position;
    };

    class Helper {
    public:
        Helper();

    protected:
        void captureMouseEvents(bool c);

        virtual void onHoverStart() = 0;

        virtual void onHoverEnd() = 0;

        virtual void onClick() = 0;

        template<typename T>
        auto getMouseListener(const T &comp) {
          return [&](const auto &event) {
            if (!mCapture) return;
            const auto &bounds{comp.globalBounds()};

            switch (event.kind) {
              case EventKind::Hover:
                if (bounds.contains(event.position)) {
                  if (!mOn) {
                    mOn = true;
                    onHoverStart();
                  }
                } else {
                  if (mOn) {
                    mOn = false;
                    onHoverEnd();
                  }
                }
                break;
              case EventKind::Click:
                if (bounds.contains(event.position)) {
                  onClick();
                }
                break;
            }
          };
        }

    private:
        bool mOn;
        bool mCapture;
    };

public:
    MouseInput();

    explicit MouseInput(std::string name);

    Kind kind() const override;

    void onStart(Entity &entity) override;

    void onEntityUpdate(Entity &entity, sf::Time dt) override;

private:
    sf::Vector2f mLastPosition;
};
}

#endif //TYPEWRITER_GAEM_MOUSEINPUT_HPP
