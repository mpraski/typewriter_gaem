//
// Created by marcin on 9/27/19.
//

#ifndef TYPEWRITER_GAEM_ACTION_HPP
#define TYPEWRITER_GAEM_ACTION_HPP

#include <SFML/Config.hpp>

namespace engine {
enum class ActionKind {
    Click,
    Press,
    None
};

struct Action final {
    Action() = default;

    Action(ActionKind kind, sf::Uint64 uid);

    ActionKind kind = ActionKind::None;
    sf::Uint64 uid = 0;
};
}
#endif //TYPEWRITER_GAEM_ACTION_HPP
