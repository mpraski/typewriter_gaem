//
// Created by marcin on 9/28/19.
//

#ifndef TYPEWRITER_GAEM_INTERACTIVE_HPP
#define TYPEWRITER_GAEM_INTERACTIVE_HPP

#include "Mesh.hpp"

namespace engine {
class Interactive final : public Component {
public:
    class Interface {
    public:
        virtual ~Interface() = default;

        virtual void onHoverStart() = 0;

        virtual void onHoverEnd() = 0;

        virtual void onClick() = 0;
    };

    enum class Event {
        Enable,
        Disable
    };

    Interactive();

    Interactive(const Interactive &);

    bool interactive();

    sf::FloatRect localBounds() const;

    sf::FloatRect globalBounds() const;

    void onHoverStart();

    void onHoverEnd();

    void onClick();

    bool dependent() const final;

    Kind kind() const final;

    void onStart(Entity &entity) final;

    void onEntityUpdate(Entity &entity, sf::Time dt) final;

private:
    bool mOn;
    bool mInteractive;

    Mesh *mMesh;
    Interface *mInterface;
};
}

#endif //TYPEWRITER_GAEM_INTERACTIVE_HPP
