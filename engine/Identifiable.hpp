//
// Created by marcin on 9/24/19.
//

#ifndef TYPEWRITER_GAEM_IDENTIFIABLE_HPP
#define TYPEWRITER_GAEM_IDENTIFIABLE_HPP

#include <SFML/Config.hpp>
#include "utilities/General.hpp"

namespace engine {
class Identifiable : sf::NonCopyable {
public:
    Identifiable();

    sf::Uint64 getUID() const noexcept;

private:
    sf::Uint64 mUID;
};
}

#endif //TYPEWRITER_GAEM_IDENTIFIABLE_HPP
