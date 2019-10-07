//
// Created by marcin on 9/28/19.
//

#ifndef TYPEWRITER_GAEM_INTERACTIVE_HPP
#define TYPEWRITER_GAEM_INTERACTIVE_HPP

#include "Component.hpp"
#include "Mesh.hpp"

namespace engine {
class Interactive final : public Component {
public:
    enum class Event {
        HoverStart,
        HoverEnd,
        Click,
        Press,
        Enable,
        Disable
    };

    Interactive();

    bool interactive();

    const std::string &getChannel() const;

    sf::FloatRect localBounds() const;

    sf::FloatRect globalBounds() const;

    void onHoverStart();

    void onHoverEnd();

    void onClick();

    void onPress();

    Kind kind() const final;

    void onStart(Entity &entity) final;

    void onEntityUpdate(Entity &entity, sf::Time dt) final;

private:
    std::string makeChannel();

private:
    bool mOn;
    bool mInteractive;

    const Mesh *mMesh;
    std::string mChannel;
};
}

#endif //TYPEWRITER_GAEM_INTERACTIVE_HPP
